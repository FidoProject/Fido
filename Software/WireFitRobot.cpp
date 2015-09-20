#include "WireFitRobot.h"

#include "LSInterpolator.h"

WireFitRobot::WireFitRobot(Task *task_) {
	task = task_;

	task->init(&simulator);
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

std::vector<int> WireFitRobot::test(int numberOfTimes, int maxIterations) {
	/// Vector Declarations
	std::vector< std::vector<double> > actions;
	std::vector< std::vector<double> > oldStates;
	std::vector<double> immediateRewards;
	std::vector< std::vector<double> > newStates;
	std::vector<double> elapsedTimes;
	std::vector<int> results(numberOfTimes);

	/// Constant definitions
	int historyLength = 10, numberOfRepetitions = 2, sleepTime = 0;

	///simulator.closeWindow();

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
			std::cout << "b: " << boltzmanExplorationLevel << "\n";

			/// Get state and perform action
			oldStates.push_back(task->getState());

			std::vector<double> action;
			if (boltzmanExplorationLevel < 0.1) action = learner.chooseBestAction(oldStates[oldStates.size() - 1]);
			else action = learner.chooseBoltzmanAction(oldStates[oldStates.size() - 1], minAction, maxAction, baseOfDimensions, boltzmanExplorationLevel);
			actions.push_back(action);

			double reward = task->performAction(action);

			/// Record state and reward
			immediateRewards.push_back(reward);
			newStates.push_back(task->getState());
			elapsedTimes.push_back(1);

			///  Train model on reward
			learner.applyReinforcementToLastAction(immediateRewards[immediateRewards.size() - 1], newStates[newStates.size() - 1], elapsedTimes[elapsedTimes.size() - 1]);
			//learner.repeated(actions, oldStates, immediateRewards, newStates, elapsedTimes, numberOfRepetitions);

			/// Learning criteria
			if (task->isTaskDone()) {
				break;
			}

			/// Erase history past allowable timesteps
			if (iter >= historyLength) {
				actions.erase(actions.begin());
				oldStates.erase(oldStates.begin());
				immediateRewards.erase(immediateRewards.begin());
				newStates.erase(newStates.begin());
				elapsedTimes.erase(elapsedTimes.begin());
			}
			
			///sf::sleep(sf::milliseconds(sleepTime));
		}
		///sf::sleep(sf::milliseconds(sleepTime));
		results[a] = iter;
	}

	return results;
}

void WireFitRobot::hyperParameterTest() {
	int minLayers = 4, maxLayers = 10, minNumberOfActions = 5, maxNumberOfActions = 13, minNeuronsPerLayer = 20, maxNeuronsPerLayer = 50;
	int numberOfTimes = 20, maxIterations = 3000;

	int layers = minLayers, numberOfActions = minNumberOfActions, neuronsPerLayer = minNeuronsPerLayer;

	while(layers <= maxLayers) {
		int stateSize = 3;
		int actionDimensions = 2;
		net::NeuralNet * network = new net::NeuralNet(stateSize, numberOfActions * (actionDimensions + 1), layers, neuronsPerLayer, "sigmoid");
		network->setOutputActivationFunction("simpleLinear");

		double backpropLearningRate = 0.1;
		double backpropMomentumTerm = 0;
		double backpropTargetError = 0.001;
		int backpropMaximumIterations = 10000;
		net::Backpropagation backprop = net::Backpropagation(backpropLearningRate, backpropMomentumTerm, backpropTargetError, backpropMaximumIterations);
		backprop.setDerivedOutputActivationFunction("simpleLinear");

		double learningRate = 0.95;
		double devaluationFactor = 0.8;

		delete learner.network;
		learner = net::WireFitQLearn(network, new net::LSInterpolator(), backprop, learningRate, devaluationFactor, actionDimensions, numberOfActions);

		boltzmanExplorationLevel = 10000;

		std::vector<int> results = test(numberOfTimes, maxIterations);
		std::cout << "l: " << layers << "; a: " << numberOfActions << "; n-per-l: " << neuronsPerLayer << "; ";
		printStats(results);

		neuronsPerLayer++;
		if(neuronsPerLayer > maxNeuronsPerLayer) {
			neuronsPerLayer = minNeuronsPerLayer;
			numberOfActions++;
		}
		if(numberOfActions > maxNumberOfActions) {
			numberOfActions = minNumberOfActions;
			layers++;
		}
	}
}

void WireFitRobot::waitForStateInput() {
	std::string response;
	std::cout << "Respond when you finish inputing the state\n";

	std::cin >> response;
}

std::vector<double> WireFitRobot::getState() {
	std::vector<double> state;
	double x, y;

	simulator.getRobotDisplacementFromEmitter(&x, &y);

	//std::cout << "s: " << x / maxDistance << ", " << y / maxDistance << ", " << (double)simulator.robot.getRotation() / 360.0 << "\n";

	state = { x / maxDistance, y / maxDistance, (double)simulator.robot.getRotation() / 360.0};
	return state;
}

double WireFitRobot::getReward() {
	double reward = 0;

	std::cout << "Enter a reward.\n";
	std::cin >> reward;

	return reward;
}

void WireFitRobot::performAction(const std::vector<double> &action) {
	std::cout << "action: " << action[0] << " " << action[1] << "\n";
	sf::Vector2f previousRobotPosition = simulator.robot.getPosition();

	simulator.robot.go(action[0]*100, action[1]*100, 2, 150);

	sf::Vector2f newRobotPosition = simulator.robot.getPosition();
	if (newRobotPosition.x < 500 + simulator.robot.getGlobalBounds().height / 2
		|| newRobotPosition.x > 1200 - simulator.robot.getGlobalBounds().height / 2
		|| newRobotPosition.y < 0 + simulator.robot.getGlobalBounds().height / 2
		|| newRobotPosition.y > 595 - simulator.robot.getGlobalBounds().height / 2) {
		//simulator.robot.setPosition(previousRobotPosition);
	}
}

void WireFitRobot::resetRobot() {
	do {
		simulator.placeEmitterInRandomPosition();
		simulator.placeRobotInRandomPosition();
	} while (simulator.getDistanceOfRobotFromEmitter() < allowableDistance);
	
	learner.resetControlPoints();
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
			median = a+min;
			break;
		}
	}

	int maxInstances = 0;
	for (int a = 0; a < histogram.size(); a++) {
		if (histogram[a] > maxInstances) {
			maxInstances = histogram[a];
			mode = a+min;
		}
		iterator += histogram[a];
	}

	std::cout << "Mean: " << mean << "; median: " << median << ";  mode: " << mode << ";  maxinstances: " << maxInstances << "\n";
}