#include "WireFitRobot.h"

WireFitRobot::WireFitRobot() {
	int stateSize = 1;
	int numberOfHiddenLayers = 3;
	int numberOfNeuronsPerHiddenLayer = 15;
	int numberOfActions = 5, actionDimensions = 2;
	net::NeuralNet * network = new net::NeuralNet(stateSize, numberOfActions * (actionDimensions + 1), numberOfHiddenLayers, numberOfNeuronsPerHiddenLayer, "sigmoid");
	network->setOutputActivationFunction("simpleLinear");
	//network->setHiddenActivationFunction("simpleLinear");

	double backpropLearningRate = 0.1;
	double backpropMomentumTerm = 0;
	double backpropTargetError = 0.0001;
	int backpropMaximumIterations = 10000;
	net::Backpropagation backprop = net::Backpropagation(backpropLearningRate, backpropMomentumTerm, backpropTargetError, backpropMaximumIterations);
	backprop.setDerivedOutputActivationFunction("simpleLinear");
	//backprop.setDerivedHiddenActivationFunction("simpleLinear");

	double learningRate = 0.95;
	double devaluationFactor = 0.4;
	learner = net::WireFitQLearn(network, backprop, learningRate, devaluationFactor, actionDimensions, numberOfActions);

	boltzmanExplorationLevel = 8;
	explorationDevaluationPerTimestep = 0.6;
}

void WireFitRobot::run(int numberOfTimeSteps) {
	std::vector< std::vector<double> > actions;
	std::vector< std::vector<double> > oldStates;
	std::vector<double> immediateRewards;
	std::vector< std::vector<double> > newStates;
	std::vector<double> elapsedTimes;

	int timesInARow = 0;
	int a;
	for(a = 0; a < numberOfTimeSteps; a++) {
		oldStates.push_back(getState());
		actions.push_back(learner.chooseRandomAction(oldStates[oldStates.size() - 1], { 0, 0 }, { 1, 1 }));
		performAction(actions[actions.size() -1 ]);
		immediateRewards.push_back(std::max(1 - abs(actions[actions.size() - 1][0] - actions[actions.size() - 1][1]), 0.0));
		newStates.push_back(oldStates[oldStates.size() - 1]);
		elapsedTimes.push_back(1);

		learner.random(actions, oldStates, immediateRewards, newStates, elapsedTimes, 4);

		std::vector<double> bestAction = learner.chooseBestAction(oldStates[oldStates.size() - 1]);
		if (std::max(1 - abs(bestAction[0] - bestAction[1]), 0.0) > 0.95) {
			timesInARow++;
			if (timesInARow == 5) break;
		} else {
			timesInARow = 0;
		}
	}

	std::cout << "a: " << a << "\n";
	
	while (true) {
		waitForStateInput();
		std::vector<double> state = getState();
		std::vector<double> action = learner.chooseBestAction(state);
		performAction(action);
		learner.applyReinforcementToLastAction(getReward(), state, 1);
	}
}

void WireFitRobot::test(int numberOfTimes, int maxIterations) {
	std::vector< std::vector<double> > actions;
	std::vector< std::vector<double> > oldStates;
	std::vector<double> immediateRewards;
	std::vector< std::vector<double> > newStates;
	std::vector<double> elapsedTimes;

	std::vector<int> results(numberOfTimes);

	simulator.closeWindow();

	for (int a = 0; a < numberOfTimes; a++) {
		oldStates.clear();
		actions.clear();
		immediateRewards.clear();
		newStates.clear();
		elapsedTimes.clear();

		int iter, timesInARow = 0;
		for (iter = 0; iter < maxIterations; iter++) {
			oldStates.push_back(getState());
			actions.push_back(learner.chooseRandomAction(oldStates[oldStates.size() - 1], { 0, 0 }, { 1, 1 }));
			performAction(actions[actions.size() - 1]);
			immediateRewards.push_back(std::max(1 - abs(actions[actions.size() - 1][0] - actions[actions.size() - 1][1]), 0.0));
			newStates.push_back(oldStates[oldStates.size() - 1]);
			elapsedTimes.push_back(1);

			learner.applyReinforcementToLastAction(immediateRewards[immediateRewards.size() - 1], newStates[newStates.size() - 1], elapsedTimes[elapsedTimes.size() - 1]);
			//learner.random(actions, oldStates, immediateRewards, newStates, elapsedTimes, 1);

			if (iter > 1) {
				actions.erase(actions.begin());
				oldStates.erase(oldStates.begin());
				immediateRewards.erase(immediateRewards.begin());
				newStates.erase(newStates.begin());
				elapsedTimes.erase(elapsedTimes.begin());
			}

			std::vector<double> bestAction = learner.chooseBestAction(oldStates[oldStates.size() - 1]);
			if (std::max(1 - abs(bestAction[0] - bestAction[1]), 0.0) > 0.95) {
				timesInARow++;
				if (timesInARow == 5) break;
			}
			else {
				timesInARow = 0;
			}
		}
		std::cout << "a: " << a << "; iter: " << iter << "\n";
		results[a] = iter;

		learner.resetControlPoints();
	}

	double mean = 0, median = 0, mode = 0;
	std::vector<int> histogram(maxIterations + 1);

	for (int a = 0; a < numberOfTimes; a++) {
		mean += (double) results[a] / (double) numberOfTimes;
		if (a == numberOfTimes / 2) median = results[a];
		histogram[results[a]]++;
	}

	int maxInstances = 0;
	for (int a = 0; a < maxIterations; a++) if (histogram[a] > maxInstances) maxInstances = histogram[a], mode = a;

	std::cout << "Mean: " << mean << "; median: " << median << ";  mode: " << mode << "\n";
}

void WireFitRobot::waitForStateInput() {
	std::string response;
	std::cout << "Respond when you finish inputing the state\n";
	std::cin >> response;
}

std::vector<double> WireFitRobot::getState() {
	std::vector<double> state;
	state.push_back(0);

	return state;
}

double WireFitRobot::getReward() {
	double reward = 0;

	std::cout << "Enter a reward.\n";
	std::cin >> reward;

	return reward;
}

void WireFitRobot::performAction(const std::vector<double> &action) {
	simulator.setMotors(action[0] * 100, action[1] * 100);
}