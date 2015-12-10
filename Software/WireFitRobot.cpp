#include "WireFitRobot.h"

#include <iostream>
#include <algorithm>

#include "LSInterpolator.h"
#include "Tasks/Task.h"
#include "NeuralNet.h"

WireFitRobot::WireFitRobot(Task *task_) {
	task = task_;

	task->getRobotParameters(&stateSize, &actionDimensions, &numberOfActions, &numberOfNeuronsPerHiddenLayer, &numberOfHiddenLayers, &boltzmanExplorationLevel, &explorationDevaluationPerTimestep, &minAction, &maxAction, &baseOfDimensions);

	net::NeuralNet * network = new net::NeuralNet(stateSize, numberOfActions * (actionDimensions + 1), numberOfHiddenLayers, numberOfNeuronsPerHiddenLayer, "sigmoid");
	network->setOutputActivationFunction("simpleLinear");

	double backpropLearningRate = 0.1;
	double backpropMomentumTerm = 0;
	double backpropTargetError = 0.001;
	int backpropMaximumIterations = 10000;
	net::Backpropagation backprop = net::Backpropagation(backpropLearningRate, backpropMomentumTerm, backpropTargetError, backpropMaximumIterations);
	backprop.setDerivedOutputActivationFunction("simpleLinear");

	double learningRate = 0.95;
	double devaluationFactor = 0.4;
	learner = net::WireFitQLearn(network, new net::LSInterpolator(), backprop, learningRate, devaluationFactor, actionDimensions, numberOfActions);
}

std::vector<int> WireFitRobot::test(int numberOfTimes, int maxIterations) {
	/// Vector Declarations
	std::vector< std::vector<double> > actions, oldStates, newStates;
	std::vector<double> immediateRewards, elapsedTimes, boltz, train;
	std::vector<int> results(numberOfTimes);

	/// Constant definitions
	int historyLength = 10, numberOfRepetitions = 2, sleepTime = 0;

	for (int a = 0; a < numberOfTimes; a++) {
		oldStates.clear();
		actions.clear();
		immediateRewards.clear();
		newStates.clear();
		elapsedTimes.clear();

		resetRobot();

		int iter;
		for (iter = 0; iter < maxIterations; iter++) {
			boltzmanExplorationLevel *= explorationDevaluationPerTimestep;

			/// Get state and perform action
			oldStates.push_back(task->getState());

			clock_t begin = clock();
			std::vector<double> action = learner.chooseBoltzmanAction(oldStates[oldStates.size() - 1], minAction, maxAction, baseOfDimensions, boltzmanExplorationLevel);
			clock_t end = clock();
			double millis = double(end - begin) / (CLOCKS_PER_SEC);
			boltz.push_back(millis);
			
			actions.push_back(action);

			double reward = task->performAction(action);

			/// Record state and reward
			immediateRewards.push_back(reward);
			newStates.push_back(task->getState());
			elapsedTimes.push_back(1);

			begin = clock();
			///  Train model on reward
			learner.applyReinforcementToLastAction(immediateRewards[immediateRewards.size() - 1], newStates[newStates.size() - 1], elapsedTimes[elapsedTimes.size() - 1]);
			//learner.repeated(actions, oldStates, immediateRewards, newStates, elapsedTimes, numberOfRepetitions);
			end = clock();
			millis = double(end - begin) / (CLOCKS_PER_SEC);
			train.push_back(millis);
			

			/// Learning criteria
			if (task->isTaskDone()) break;

			// Train on past data. If a < 0 is the exit condition of the for loop, this is essentially commented out
			for (int a = 0; a < 0; a++) {
				action = learner.chooseBoltzmanAction(task->getState(), minAction, maxAction, baseOfDimensions, boltzmanExplorationLevel);
				task->performAction(action);
			}

			/// Erase history past allowable timesteps
			if (iter >= historyLength) {
				actions.erase(actions.begin());
				oldStates.erase(oldStates.begin());
				immediateRewards.erase(immediateRewards.begin());
				newStates.erase(newStates.begin());
				elapsedTimes.erase(elapsedTimes.begin());
			}
		}
		///sf::sleep(sf::milliseconds(sleepTime));
		results[a] = iter;
		std::cout << "iter: " << iter << "\n";
	}

	printStats(std::vector<double>(results.begin(), results.end()));
	printStats(boltz);
	printStats(train);

	return results;
}

void WireFitRobot::runIteration() {
	std::vector<double> state = task->getState();
	std::vector<double> action = learner.chooseBoltzmanAction(state, minAction, maxAction, baseOfDimensions, boltzmanExplorationLevel);
	double reward = task->performAction(action);
	std::vector<double> newState = task->getState();
	learner.applyReinforcementToLastAction(reward, newState, 0);
}

void WireFitRobot::hyperParameterTest() {
	int minLayers = 4, maxLayers = 10, minNumberOfActions = 5, maxNumberOfActions = 13, minNeuronsPerLayer = 20, maxNeuronsPerLayer = 50;
	int numberOfTimes = 20, maxIterations = 3000;

	int hpLayers = minLayers, hpNumberOfActions = minNumberOfActions, hpNeuronsPerLayer = minNeuronsPerLayer;

	while(hpLayers <= maxLayers) {
		/// Setup Robot
		task->getRobotParameters(&stateSize, &actionDimensions, &numberOfActions, &numberOfNeuronsPerHiddenLayer, &numberOfHiddenLayers, &boltzmanExplorationLevel, &explorationDevaluationPerTimestep, &minAction, &maxAction, &baseOfDimensions);

		net::NeuralNet * network = new net::NeuralNet(stateSize, hpNumberOfActions * (actionDimensions + 1), hpLayers, hpNeuronsPerLayer, "sigmoid");
		network->setOutputActivationFunction("simpleLinear");

		double backpropLearningRate = 0.1;
		double backpropMomentumTerm = 0;
		double backpropTargetError = 0.001;
		int backpropMaximumIterations = 10000;
		net::Backpropagation backprop = net::Backpropagation(backpropLearningRate, backpropMomentumTerm, backpropTargetError, backpropMaximumIterations);
		backprop.setDerivedOutputActivationFunction("simpleLinear");

		double learningRate = 0.95;
		double devaluationFactor = 0.4;
		delete learner.network;
		learner = net::WireFitQLearn(network, new net::LSInterpolator(), backprop, learningRate, devaluationFactor, actionDimensions, hpNumberOfActions);

		/// Carry out test
		std::vector<int> results = test(numberOfTimes, maxIterations);
		std::cout << "l: " << hpNeuronsPerLayer << "; a: " << hpNumberOfActions << "; n-per-l: " << hpNeuronsPerLayer << "; ";
		printStats(std::vector<double>(results.begin(), results.end()));

		/// Update hyperparameters
		hpNeuronsPerLayer++;
		if(hpNeuronsPerLayer > maxNeuronsPerLayer) {
			hpNeuronsPerLayer = minNeuronsPerLayer;
			hpNumberOfActions++;
		}
		if(hpNumberOfActions > maxNumberOfActions) {
			hpNumberOfActions = minNumberOfActions;
			hpLayers++;
		}
	}
}

void WireFitRobot::resetRobot() {
	learner.resetControlPoints();
	task->reset();
}

bool sortFunction(double a, double b) {
	return a > b;
}

void WireFitRobot::printStats(std::vector<double> data) {
	double mean = 0, median = 0;

	double min = 9999999, max = -9999999;
	for (int a = 0; a < data.size(); a++) {
		if (data[a] > max) max = data[a];
		if (data[a] < min) min = data[a];
	}
	double range = max - min;

	double totalValue = 0;
	for (int a = 0; a < data.size(); a++) {
		mean += (double)data[a] / (double)data.size();
	}

	std::sort(data.begin(), data.end(), sortFunction);

	median = data[data.size() / 2];

	std::cout << "Mean: " << mean << "; median: " << median << "\n";
}
