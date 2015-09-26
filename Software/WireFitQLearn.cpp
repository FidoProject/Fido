#include "WireFitQLearn.h"

#include <random>
#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <SFML/Graphics.hpp>

#include "Interpolator.h"
#include "NeuralNet.h"

using namespace net;

WireFitQLearn::WireFitQLearn(NeuralNet *modelNetwork, Interpolator *interpolator_, Backpropagation backprop_, double learningRate_, double devaluationFactor_, int actionDimensions_, int numberOfWires_) {
    backprop = backprop_;
    learningRate = learningRate_;
    devaluationFactor = devaluationFactor_;
    actionDimensions = actionDimensions_;
    numberOfWires = numberOfWires_;
    lastAction = std::vector<double>(actionDimensions);
    network = modelNetwork;
	interpolator = interpolator_;
    
    scalingFactorToMillis = 1;
	biasMovingAverage = 0;
	rewardIterations = 0;

    controlPointsGDErrorTarget = 0.001;
    controlPointsGDLearningRate = 0.1;
    controlPointsGDMaxIterations = 10000;

    if(network->numOutputs != numberOfWires * (actionDimensions+1)) {
        std::cout << "Neural network incorrectly formatted!\n";
        throw 1;
    }
}

WireFitQLearn::WireFitQLearn(std::string filename) {
    std::ifstream input(filename);
    if(input.is_open()) {
        input >> learningRate >> devaluationFactor;
        input >> actionDimensions >> numberOfWires;
        input >> scalingFactorToMillis >> controlPointsGDErrorTarget >> controlPointsGDLearningRate >> controlPointsGDMaxIterations >> biasMovingAverage >> rewardIterations;
        
        lastAction = std::vector<double>(actionDimensions);
        for(int a = 0; a < actionDimensions; a++) {
            double temp;
            input >> temp;
            lastAction[a] = temp;
            
        }
        
        backprop = Backpropagation(input);
		interpolator = Interpolator::getAnyInterpolatorFromFile(&input);
        network = new NeuralNet(input);
        
        input.close();

        if(network->numOutputs != numberOfWires * (actionDimensions+1)) {
            std::cout << "Neural network incorrectly formatted!\n";
            throw 1;
        }
    } else {
        std::cout << "Could not retrieve wirefitqlearn from file\n";
        throw 1;
    }
}

WireFitQLearn::WireFitQLearn() { }

std::vector<double> WireFitQLearn::chooseBestAction(std::vector<double> currentState) {
    std::vector<double> action = bestAction(currentState);
    lastAction = action;
    lastState = currentState;
    return action;
}

std::vector<double> WireFitQLearn::chooseBoltzmanAction(std::vector<double> currentState,
	const std::vector<double> &minAction,
	const std::vector<double> &maxAction,
	int baseOfDimensions,
	double explorationConstant) {

	if (explorationConstant < 0.01) explorationConstant = 0.01;

    double determiner = (double)rand() / (double)RAND_MAX;
    std::vector<double> exponentTerms;
    double sumOfExponentTerms = 0;
    std::vector<Wire> wires = getSetOfWires(currentState, minAction, maxAction, baseOfDimensions);
    
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

std::vector<double> WireFitQLearn::chooseRandomAction(const std::vector<double> &state, const std::vector<double> &minAction, const std::vector<double> &maxAction) {
	if (minAction.size() != maxAction.size()) {
		std::cout << "Min action and max action sizes don't match\n";
		throw 1;
	}
	
	std::vector<double> action(minAction.size());
	for (int a = 0; a < minAction.size(); a++) action[a] = minAction[a] + ((double)rand() / RAND_MAX) * (maxAction[a] - minAction[a]);
	
	lastAction = action;
	lastState = state;

	return action;
}

void WireFitQLearn::applyReinforcementToLastAction(double reward, std::vector<double> newState, double elapsedTimeMillis) {
	std::vector<Wire> controlWires = getWires(lastState);
	double newRewardForLastAction = getQValue(reward, lastState, newState, lastAction, elapsedTimeMillis, controlWires);
	double oldReward = interpolator->getReward(controlWires, lastAction);

	Wire correctWire = {lastAction, newRewardForLastAction};
	std::vector<Wire> newContolWires = newControlWires(correctWire, controlWires);

	updateBias(oldReward, newRewardForLastAction);

    backprop.trainOnData(network, {lastState}, {getRawOutput(newContolWires)});
}

void WireFitQLearn::resetControlPoints() {
	NeuralNet *temp = new NeuralNet(network);
	delete network;
	network = temp;
}

void WireFitQLearn::repeated(const std::vector< std::vector<double> > &actions,
	const std::vector< std::vector<double> > &oldStates,
	const std::vector<double> &immediateRewards,
	const std::vector< std::vector<double> > &newStates,
	const std::vector<double> &elapsedTimes,
	int numberOfIterations) {

	if(actions.size() != oldStates.size() || oldStates.size() != immediateRewards.size() || immediateRewards.size() != newStates.size() || newStates.size() != elapsedTimes.size()) {
		std::cout << "Varying lengths of historical data vectors\n";
		throw 1;
	}

	for(int a = 0; a < numberOfIterations; a++) {
		for(int b = 0; b < actions.size(); b++) {
			std::vector<Wire> controlWires = getWires(oldStates[b]);
			double qValue = getQValue(immediateRewards[b], oldStates[b], newStates[b], actions[b], elapsedTimes[b], controlWires);

			Wire correctWire = { actions[b], qValue };
			std::vector<Wire> newContolWires = newControlWires(correctWire, controlWires);

			backprop.trainOnData(network, { oldStates[b] }, { getRawOutput(newContolWires) });
		}
	}
}

void WireFitQLearn::shuffle(const std::vector< std::vector<double> > &actions,
		const std::vector< std::vector<double> > &oldStates,
		const std::vector<double> &immediateRewards,
		const std::vector< std::vector<double> > &newStates,
		const std::vector<double> &elapsedTimes,
		int numberOfIterations) {
	
	if (actions.size() != oldStates.size() || oldStates.size() != immediateRewards.size() || immediateRewards.size() != newStates.size() || newStates.size() != elapsedTimes.size()) {
		std::cout << "Varying lengths of historical data vectors\n";
		throw 1;
	}

	/// Struct for holding a historical data point
	struct DataPoint {
		std::vector<double> action;
		std::vector<double> oldState;
		double immediateReward;
		std::vector<double> newState;
		double elapsedTime;
	};

	/// Organize the vectors given as parameters into a vector of DataPoints so the data can be shuffled
	std::vector<DataPoint> dataPoints(actions.size());
	for (int a = 0; a < actions.size(); a++) dataPoints[a] = {actions[a], oldStates[a], immediateRewards[a], newStates[a], elapsedTimes[a]};

	for (int a = 0; a < numberOfIterations; a++) {
		// obtain a time-based seed:
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(dataPoints.begin(), dataPoints.end(), std::default_random_engine(seed));

		for (int b = 0; b < dataPoints.size(); b++) {
			std::vector<Wire> controlWires = getWires(dataPoints[b].oldState);
			double qValue = getQValue(dataPoints[b].immediateReward, dataPoints[b].oldState, dataPoints[b].newState, dataPoints[b].action, dataPoints[b].elapsedTime, controlWires);

			Wire correctWire = { dataPoints[b].action, qValue };
			std::vector<Wire> newContolWires = newControlWires(correctWire, controlWires);

			backprop.trainOnData(network, { dataPoints[b].oldState }, { getRawOutput(newContolWires) });
		}
	}
}

void WireFitQLearn::storeWireFitQLearn(std::string filename) {
    std::ofstream output(filename);
    if(output.is_open()) {
        output << learningRate << " " << devaluationFactor << "\n";
        output << actionDimensions << " " << numberOfWires << "\n";
        output << scalingFactorToMillis << " " << controlPointsGDErrorTarget << " " << controlPointsGDLearningRate << " " << controlPointsGDMaxIterations << " " << biasMovingAverage << " " << rewardIterations << "\n";
        
        for(int a = 0; a < lastAction.size(); a++) output << lastAction[a] << " ";
        output << "\n";
        
        backprop.storeBackpropagationWithStream(output);
		interpolator->storeInterpolator(&output);
        network->storeNetWithStream(output);
        
        output.close();
    } else {
        std::cout << "Could not store wirefitqlearn in file\n";
        throw 1;
    }
}

std::vector<Wire> WireFitQLearn::getWires(std::vector<double> state) {
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

std::vector<Wire> WireFitQLearn::getSetOfWires(const std::vector<double> &state,
	const std::vector<double> &minAction,
	const std::vector<double> &maxAction,
	int baseOfDimensions) {

	if (minAction.size() != maxAction.size() || minAction.size() != actionDimensions) {
		std::cout << "Mismatching minAction max action \n";
		throw 1;
	}
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


double WireFitQLearn::highestReward(std::vector<double> state) {
    std::vector<Wire> wires = getWires(state);
    double bestReward = -99999;
    
    for(int a = 0; a < wires.size(); a++) if(wires[a].reward > bestReward) bestReward = wires[a].reward;
    
    return bestReward;
}

std::vector<double> WireFitQLearn::bestAction(std::vector<double> state) {
    std::vector<Wire> wires = getWires(state);
    double bestReward = -99999;
    std::vector<double> *bestAction = 0;
    
    for(int a = 0; a < wires.size(); a++) {
        if(wires[a].reward > bestReward) {
            bestAction = &wires[a].action;
            bestReward = wires[a].reward;
        }
    }
    
    return *bestAction;
}

double WireFitQLearn::getQValue(double reward,
	const std::vector<double> &oldState,
	const std::vector<double> &newState,
	const std::vector<double> &action,
	double elapsedTimeMillis,
	const std::vector<Wire> &controlWires)
{
	
	double scalingFactor = scalingFactorToMillis * elapsedTimeMillis;

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


void WireFitQLearn::graphInterpolatorFunction(const std::vector<Wire> &controlWires, double minAction, double maxAction, double targetAction) {
	double xSize = 1000, ySize = 800;
	sf::RenderWindow window(sf::VideoMode(xSize, ySize), "Interpolator Graph");
	double numberOfDots = 100;

	for (int a = 0; a < controlWires.size(); a++) {
		if (controlWires[a].action[0] > maxAction) maxAction = controlWires[a].action[0];
		if (controlWires[a].action[0] < minAction) minAction = controlWires[a].action[0];
	}

	double increment = (maxAction - minAction) / numberOfDots;
	double xScale = (xSize - 60) / (maxAction - minAction);

	double maxReward = -99999999, minReward = 99999999;
	std::vector<double> rewards(numberOfDots);
	for (int a = 0; a < numberOfDots; a++) {
		rewards[a] = interpolator->getReward(controlWires, { a*increment });
		if (rewards[a] > maxReward) maxReward = rewards[a];
		if (rewards[a] < minReward) minReward = rewards[a];
	}
	for (int a = 0; a < controlWires.size(); a++) {
		if (controlWires[a].reward > maxReward) maxReward = controlWires[a].reward;
		if (controlWires[a].reward < minReward) minReward = controlWires[a].reward;
	}

	double yScale = (ySize - 60) / (maxReward - minReward);

	window.clear();
	sf::CircleShape shape(5.f);
	shape.setFillColor(sf::Color::Green);
	for (int a = 0; a < numberOfDots; a++) {
		shape.setPosition((a*increment - minAction) * xScale + 30, (maxReward - rewards[a])*yScale + 30);
		window.draw(shape);
	}

	shape.setRadius(10.f);
	shape.setFillColor(sf::Color::Red);
	for (int a = 0; a < controlWires.size(); a++) {
		shape.setPosition((controlWires[a].action[0] - minAction) * xScale + 30, (maxReward - controlWires[a].reward)*yScale + 30);
		window.draw(shape);
	}

	shape.setFillColor(sf::Color::Magenta);
	shape.setPosition((targetAction - minAction) * xScale + 30, (maxReward - interpolator->getReward(controlWires, { targetAction }))*yScale + 30);
	window.draw(shape);

	window.display();


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}
}

void WireFitQLearn::updateBias(double oldReward, double correctReward) {
	rewardIterations++;
	biasMovingAverage -= biasMovingAverage / (double)rewardIterations;
	biasMovingAverage += pow(correctReward - oldReward, 2) / (double)rewardIterations;
}