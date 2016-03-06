#include "WireFitRobot.h"

#include <iostream>
#include <algorithm>
#include <time.h>
#include <chrono>
#include <thread>

#include "LSInterpolator.h"
#include "Tasks/Task.h"
#include "NeuralNet.h"

WireFitRobot::WireFitRobot(Task *task_) {
	task = task_;

	task->getRobotParameters(&stateSize, &actionDimensions, &numberOfActions, &numberOfNeuronsPerHiddenLayer, &numberOfHiddenLayers, &startingExplorationLevel, &explorationDevaluationPerTimestep, &minAction, &maxAction, &baseOfDimensions);

	/*net::NeuralNet * network = new net::NeuralNet(stateSize, numberOfActions * (actionDimensions + 1), numberOfHiddenLayers, numberOfNeuronsPerHiddenLayer, "sigmoid");
	network->setOutputActivationFunction("simpleLinear");

	double backpropLearningRate = 0.1;
	double backpropMomentumTerm = 0;
	double backpropTargetError = 0.001;
	int backpropMaximumIterations = 10000;
	net::Backpropagation backprop = net::Backpropagation(backpropLearningRate, backpropMomentumTerm, backpropTargetError, backpropMaximumIterations);
	backprop.setDerivedOutputActivationFunction("simpleLinear");

	double learningRate = 0.95;
	double devaluationFactor = 0.4;
	learner = net::WireFitQLearn(network, new net::LSInterpolator(), backprop, learningRate, devaluationFactor, actionDimensions, numberOfActions);*/
}

std::vector<int> WireFitRobot::runTrials(int numberOfTimes, int maxIterations) {
	std::vector<int> results(numberOfTimes);
	std::vector<double> boltz, train;

	for (int a = 0; a < numberOfTimes; a++) {
		resetRobot();

		int iter;
		for (iter = 0; iter < maxIterations; iter++) {
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
			//std::cout << currentExplorationLevel << "; "; std::cout.flush();

			currentExplorationLevel *= explorationDevaluationPerTimestep;
			if(currentExplorationLevel < 0.1) currentExplorationLevel = 0.1;

			std::vector<double> state = task->getState();
			clock_t begin = clock();
			std::vector<double> action = learner.chooseBoltzmanAction(state, minAction, maxAction, baseOfDimensions, currentExplorationLevel);
			clock_t end = clock();
			boltz.push_back(double(end - begin) / (CLOCKS_PER_SEC));

			double reward = task->performAction(action);
			std::vector<double> newState = task->getState();

			begin = clock();
			learner.applyReinforcementToLastAction(reward, newState, 0);
			end = clock();
			train.push_back(double(end - begin) / (CLOCKS_PER_SEC));

			if (task->isTaskDone()) break;
		}

		results[a] = iter;
	}

	printStats(std::vector<double>(results.begin(), results.end()));
	printStats(boltz);
	printStats(train);
	return results;
}

void WireFitRobot::runIteration() {
	currentExplorationLevel *= explorationDevaluationPerTimestep;

	std::vector<double> state = task->getState();
	std::vector<double> action = learner.chooseBoltzmanAction(state, minAction, maxAction, baseOfDimensions, currentExplorationLevel);
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
		task->getRobotParameters(&stateSize, &actionDimensions, &numberOfActions, &numberOfNeuronsPerHiddenLayer, &numberOfHiddenLayers, &startingExplorationLevel, &explorationDevaluationPerTimestep, &minAction, &maxAction, &baseOfDimensions);

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
		std::vector<int> results = runTrials(numberOfTimes, maxIterations);
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
	currentExplorationLevel = startingExplorationLevel;
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
