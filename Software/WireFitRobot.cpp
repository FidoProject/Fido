#include "WireFitRobot.h"

#include "LSInterpolator.h"

WireFitRobot::WireFitRobot() {
	int stateSize = 1;
	int numberOfHiddenLayers = 3;
	int numberOfNeuronsPerHiddenLayer = 15;
	int numberOfActions = 5, actionDimensions = 2;
	net::NeuralNet * network = new net::NeuralNet(stateSize, numberOfActions * (actionDimensions + 1), numberOfHiddenLayers, numberOfNeuronsPerHiddenLayer, "sigmoid");
	network->setOutputActivationFunction("simpleLinear");

	double backpropLearningRate = 0.1;
	double backpropMomentumTerm = 0;
	double backpropTargetError = 0.0001;
	int backpropMaximumIterations = 10000;
	net::Backpropagation backprop = net::Backpropagation(backpropLearningRate, backpropMomentumTerm, backpropTargetError, backpropMaximumIterations);
	backprop.setDerivedOutputActivationFunction("simpleLinear");

	double learningRate = 0.95;
	double devaluationFactor = 0.4;
	learner = net::WireFitQLearn(network, new net::LSInterpolator(), backprop, learningRate, devaluationFactor, actionDimensions, numberOfActions);

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

		learner.repeated(actions, oldStates, immediateRewards, newStates, elapsedTimes, 4);

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

	for (int a = 0; a < numberOfTimes; a++) {
		oldStates.clear();
		actions.clear();
		immediateRewards.clear();
		newStates.clear();
		elapsedTimes.clear();

		int iter;
		for (iter = 0; iter < maxIterations; iter++) {
			boltzmanExplorationLevel /= 1.1;
			oldStates.push_back(getState());
			actions.push_back(learner.chooseBoltzmanAction(oldStates[oldStates.size() - 1], { -1, -1 }, {1, 1}, 200, boltzmanExplorationLevel));
			performAction(actions[actions.size() - 1]);

			TDVect imu = simulator.getCompass();
			double reward = 1 - (simulator.getDistanceOfRobotFromEmitter() / 982);
			immediateRewards.push_back(reward);
			newStates.push_back(getState());
			elapsedTimes.push_back(1);

			learner.applyReinforcementToLastAction(immediateRewards[immediateRewards.size() - 1], newStates[newStates.size() - 1], elapsedTimes[elapsedTimes.size() - 1]);
			learner.repeated(actions, oldStates, immediateRewards, newStates, elapsedTimes, 2);

			std::vector<double> bestAction = learner.chooseBestAction(oldStates[oldStates.size() - 1]);
			if (simulator.getDistanceOfRobotFromEmitter() < 150) {
				break;
			}

			if (iter >= 100) {
				actions.erase(actions.begin());
				oldStates.erase(oldStates.begin());
				immediateRewards.erase(immediateRewards.begin());
				newStates.erase(newStates.begin());
				elapsedTimes.erase(elapsedTimes.begin());
			}
		}
		sf::sleep(sf::milliseconds(500));
		std::cout << "a: " << a << "; iter: " << iter << "\n";
		results[a] = iter;

		resetRobot();
	}

	printStats(results);
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

void WireFitRobot::resetRobot() {
	learner.resetControlPoints();
	simulator.placeEmitterInRandomPosition();
	simulator.placeRobotInRandomPosition();
	performAction({ 0, 0 });
	boltzmanExplorationLevel = 8;
}

void WireFitRobot::printStats(std::vector<int> data) {
	double mean = 0, median = 0, mode = 0;

	int min = 9999999, max = -9999999;
	for (int a = 0; a < data.size(); a++) {
		if (data[a] > max) max = data[a];
		if (data[a] < min) min = data[a];
	}
	int range = max - min;

	std::vector<int> histogram(range + 1);

	for (int a = 0; a < data.size(); a++) {
		mean += (double)data[a] / (double)data.size();
		histogram[data[a] - min]++;
	}

	int iterator = 0;
	for (int a = 0; a <= histogram.size(); a++) {
		iterator += histogram[a];
		if (iterator > data.size() / 2) {
			median = a;
			break;
		}
	}

	int maxInstances = 0;
	for (int a = 0; a <= histogram.size(); a++) {
		if (histogram[a] > maxInstances) {
			maxInstances = histogram[a];
			mode = a;
		}
		iterator += histogram[a];
	}

	std::cout << "Mean: " << mean << "; median: " << median << ";  mode: " << mode << ";  maxinstances: " << maxInstances << "\n";
}