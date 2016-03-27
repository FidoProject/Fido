#include "../include/WireFitQLearn.h"

#include <random>
#include <iostream>
#include <thread>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <random>
#include <cfloat>

#include "../include/Interpolator.h"
#include "../include/NeuralNet.h"

using namespace rl;

WireFitQLearn::WireFitQLearn(int stateDimensions, int actionDimensions_, int numHiddenLayers, int numNeuronsPerHiddenLayer, int numberOfWires_, Action minAction_, Action maxAction_, int baseOfDimensions_, Interpolator *interpolator_, net::Backpropagation backprop_, double learningRate_, double devaluationFactor_) {

	if(minAction_.size() != actionDimensions_) {
		std::cout << "Min action does not have same dimensions as action dimensions parameter!\n"; std::cout.flush();
		throw 1;
	}
	if(maxAction_.size() != actionDimensions_) {
		std::cout << "Max action does not have same dimensions as action dimensions parameter!\n"; std::cout.flush();
		throw 1;
	}

	backprop = backprop_;
	learningRate = learningRate_;
	devaluationFactor = devaluationFactor_;
	actionDimensions = actionDimensions_;
	numberOfWires = numberOfWires_;
	lastAction = std::vector<double>(actionDimensions);
	interpolator = interpolator_;
	minAction = minAction_;
	maxAction = maxAction_;

	network = new net::NeuralNet(stateDimensions, numberOfWires_ * (actionDimensions_+1), numHiddenLayers, numNeuronsPerHiddenLayer, "sigmoid");
	network->setOutputActivationFunction("simpleLinear");
	
	scalingFactorToMillis = 1;
	rewardIterations = 0;

	controlPointsGDErrorTarget = 0.001;
	controlPointsGDLearningRate = 0.1;
	controlPointsGDMaxIterations = 10000;

	baseOfDimensions = baseOfDimensions_;
}

WireFitQLearn::WireFitQLearn() {

}

WireFitQLearn::WireFitQLearn(std::ifstream *input) {
	if(input->is_open()) {
		*input >> learningRate >> devaluationFactor;
		*input >> actionDimensions >> numberOfWires;
		*input >> scalingFactorToMillis >> controlPointsGDErrorTarget >> controlPointsGDLearningRate >> controlPointsGDMaxIterations >> rewardIterations >> baseOfDimensions;

		double temp;
		minAction = Action(actionDimensions);
		for(int a = 0; a < actionDimensions; a++) {
			*input >> temp;
			minAction[a] = temp;
		}

		maxAction = Action(actionDimensions);
		for(int a = 0; a < actionDimensions; a++) {
			*input >> temp;
			maxAction[a] = temp;
		}

		lastAction = Action(actionDimensions);
		for(int a = 0; a < actionDimensions; a++) {
			*input >> temp;
			lastAction[a] = temp;
		}
		
		backprop = net::Backpropagation(input);
		interpolator = Interpolator::getAnyInterpolatorFromFile(input);
		network = new net::NeuralNet(input);
		
		input->close();

		if(network->numberOfOutputs() != numberOfWires * (actionDimensions+1)) {
			std::cout << "WireFitQLearn incorrectly formatted!\n";
			throw 1;
		}
	} else {
		std::cout << "Could not retrieve WireFitQLearn from file\n";
		throw 1;
	}
}

Action WireFitQLearn::chooseBestAction(State currentState) {
	std::vector<double> action = bestAction(currentState);
	lastAction = action;
	lastState = currentState;
	return action;
}

Action WireFitQLearn::chooseBoltzmanAction(State currentState,
	double explorationConstant) {

	if (explorationConstant < 0.01) explorationConstant = 0.01;

	double determiner = (double)rand() / (double)RAND_MAX;
	std::vector<double> exponentTerms;
	double sumOfExponentTerms = 0;
	std::vector<Wire> wires = getSetOfWires(currentState, baseOfDimensions);
	
	for(int a = 0; a < wires.size(); a++) {
		double exponentTerm = exp(wires[a].reward / explorationConstant);
		exponentTerms.push_back(exponentTerm);
		sumOfExponentTerms += exponentTerm;
	}
	
	double sumOfProbabilities = 0;
	for(int a = 0; a < wires.size(); a++) {
		sumOfProbabilities += (exponentTerms[a] / sumOfExponentTerms);
		if(sumOfProbabilities >= determiner) {
			lastAction = wires[a].action;
			lastState = currentState;
			return wires[a].action;
		}
	}
	
	/// Incase a floating point error resulted in no wire being chosen
	lastAction = wires[wires.size() - 1].action;
	lastState = currentState;
	return wires[wires.size() - 1].action;
	
}

void WireFitQLearn::applyReinforcementToLastAction(double reward, State newState) {
	std::vector<Wire> controlWires = getWires(lastState);
	double newRewardForLastAction = getQValue(reward, lastState, newState, lastAction, controlWires);
	//double oldRewa2d = interpolator->getReward(controlWires, lastAction);

	histories.push_back(History(lastState, newState, lastAction, reward));
	if(histories.size() > 100) {
		histories.erase(histories.begin());
	}

	Wire correctWire = {lastAction, newRewardForLastAction};
	std::vector<Wire> newContolWires = newControlWires(correctWire, controlWires);

	std::vector< std::vector<double> > input(1, lastState);
	std::vector< std::vector<double> > correctOutput(1, getRawOutput(newContolWires));

	for(int a = 0; a < (histories.size() < 1 ? histories.size() : 1); a++) {
		int index = floor((double(rand()) / double(RAND_MAX))*histories.size());
		History randomHistory = histories[index];
		std::vector<Wire> randomControlWires = getWires(randomHistory.initialState);
		input.push_back(randomHistory.initialState);
		correctOutput.push_back(getRawOutput(newControlWires({randomHistory.action, getQValue(randomHistory.reward, randomHistory.initialState, randomHistory.newState, randomHistory.action, randomControlWires)}, randomControlWires)));
	}

	backprop.train(network, input, correctOutput);
}

void WireFitQLearn::reset() {
	network->randomizeWeights();
	histories.clear();
}

void WireFitQLearn::store(std::ofstream *output) {
	if(output->is_open()) {
		*output << learningRate << " " << devaluationFactor << "\n";
		*output << actionDimensions << " " << numberOfWires << "\n";
		*output << scalingFactorToMillis << " " << controlPointsGDErrorTarget << " " << controlPointsGDLearningRate << " " << controlPointsGDMaxIterations << " " << rewardIterations <<  " " << baseOfDimensions << "\n";
		
		for(int a = 0; a < minAction.size(); a++) *output << minAction[a] << " ";
		for(int a = 0; a < maxAction.size(); a++) *output << maxAction[a] << " ";
		for(int a = 0; a < lastAction.size(); a++) *output << lastAction[a] << " ";
		
		backprop.store(output);
		interpolator->store(output);
		network->store(output);
		
		output->close();
	} else {
		std::cout << "Could not store wirefitqlearn in file\n";
		throw 1;
	}
}

std::vector<Wire> WireFitQLearn::getWires(State state) {
	std::vector<Wire> wires(numberOfWires);
	std::vector<double> rawOutput = network->getOutput(state);
	
	for(int a = 0; a < numberOfWires; a++) {
		int currentIndex = a * (actionDimensions + 1);
		Wire wire;
		wire.action = std::vector<double>(actionDimensions);
		for(int b = 0; b < actionDimensions; b++) wire.action[b] = rawOutput[currentIndex + b];
		
		wire.reward = rawOutput[currentIndex + actionDimensions];
		
		wires[a] = wire;
	}
	
	return wires;
}

std::vector<Wire> WireFitQLearn::getSetOfWires(const State &state,
	int baseOfDimensions) {

	if (baseOfDimensions <= 1) {
		std::cout << "Value of baseOfDimensions is too small\n";
		throw 1;
	}

	std::vector<Wire> controlWires = getWires(state);

	std::vector<double> scaleVector(minAction.size());
	for (int a = 0; a < minAction.size(); a++) {
		scaleVector[a] = (maxAction[a] - minAction[a]) / ((double)baseOfDimensions - 1);
	}

	int numberOfWiresReturned = pow(baseOfDimensions, actionDimensions);
	std::vector<Wire> wires(numberOfWiresReturned);
	
	std::vector<int> iteratorVector(actionDimensions);
	for (int a = 0; a < iteratorVector.size(); a++) iteratorVector[a] = 0;

	for (int a = 0; a < numberOfWiresReturned; a++) {
		Wire wire;
		for (int actionDimension = 0; actionDimension < minAction.size(); actionDimension++) {
			wire.action.push_back(iteratorVector[actionDimension] * scaleVector[actionDimension] + minAction[actionDimension]);
		}
		wire.reward = interpolator->getReward(controlWires, wire.action);
		wires[a] = (wire);

		/// Increment iterator vector
		iteratorVector[iteratorVector.size() - 1]++;
		for (int iteratorVectorIndex = iteratorVector.size() - 1; iteratorVectorIndex >= 0; iteratorVectorIndex--) {
			if (iteratorVector[iteratorVectorIndex] >= baseOfDimensions) {
				iteratorVector[iteratorVectorIndex] = 0;
				if(iteratorVectorIndex > 0)iteratorVector[iteratorVectorIndex - 1]++;
			}
		}
	}
	
	return wires;
}

std::vector<double> WireFitQLearn::getRawOutput(std::vector<Wire> wires) {
	std::vector<double> rawOutput(wires.size() * (actionDimensions + 1));
	
	for(int a = 0; a < wires.size(); a++) {
		int currentIndex = a * (actionDimensions + 1);
		Wire wire = wires[a];
		for(int b = 0; b < wire.action.size(); b++) rawOutput[currentIndex + b] = wire.action[b];
		
		rawOutput[currentIndex + wire.action.size()] = wire.reward;
	}
	
	return rawOutput;
}


double WireFitQLearn::highestReward(State state) {
	std::vector<Wire> wires = getWires(state);
	double bestReward = -99999;
	
	for(int a = 0; a < wires.size(); a++) if(wires[a].reward > bestReward) bestReward = wires[a].reward;
	
	return bestReward;
}

Action WireFitQLearn::bestAction(State state) {
	std::vector<Wire> wires = getWires(state);
	double bestReward = DBL_MIN;
	std::vector<double> *bestAction = &wires[0].action;
	
	for(int a = 0; a < wires.size(); a++) {
		if(wires[a].reward > bestReward) {
			bestAction = &wires[a].action;
			bestReward = wires[a].reward;
		}
	}
	
	return *bestAction;
}

double WireFitQLearn::getQValue(double reward,
	const State &oldState,
	const State &newState,
	const Action &action,
	const std::vector<Wire> &controlWires)
{
	
	double scalingFactor = scalingFactorToMillis;

	/// Update Q value according to adaptive learning
	double oldRewardForLastAction = interpolator->getReward(controlWires, action);
	//double feedback = ((1/scalingFactor)*( reward + (pow(devaluationFactor, scalingFactor)*highestReward(newState)) )) + (1 - 1/scalingFactor) * highestReward(oldState);
	double feedback = reward + devaluationFactor * highestReward(newState);
	double qValue = ((1 - learningRate) * oldRewardForLastAction) + (learningRate*feedback);

	return qValue;
}

std::vector<Wire> WireFitQLearn::newControlWires(const Wire &correctWire, std::vector<Wire> controlWires) {
	double error = 0;
	int iterations = 0;
	
	do {
		for(int a = 0; a < controlWires.size(); a++) {
			double deltaReward = -2 * (-interpolator->getReward(controlWires, correctWire.action) + correctWire.reward)*interpolator->rewardDerivative(correctWire.action, controlWires[a], controlWires);
			controlWires[a].reward = controlWires[a].reward - controlPointsGDLearningRate*deltaReward;
			for(int b = 0; b < controlWires[a].action.size(); b++) {
				double deltaActionTerm = -2 * (-interpolator->getReward(controlWires, correctWire.action) + correctWire.reward)*interpolator->actionTermDerivative(correctWire.action[b], controlWires[a].action[b], correctWire.action, controlWires[a], controlWires);
				controlWires[a].action[b] = controlWires[a].action[b] - controlPointsGDLearningRate*deltaActionTerm;
			}
		}
		
		error = pow(correctWire.reward - interpolator->getReward(controlWires, correctWire.action), 2);
		iterations++;
	} while(error > controlPointsGDErrorTarget && iterations < controlPointsGDMaxIterations);

	return controlWires;

}