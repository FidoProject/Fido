#include "Robot.h"

#include <iostream>
#include <algorithm>
#include <time.h>
#include <chrono>
#include <thread>

#include "LSInterpolator.h"
#include "Tasks/Task.h"
#include "NeuralNet.h"
#include "WireFitRobot.h"

Robot::Robot(Task *task_) {
	task = task_;

	int dummy;

	task->getRobotParameters(&stateSize, &actionDimensions, &dummy, &numberOfNeuronsPerHiddenLayer, &numberOfHiddenLayers, &startingExplorationLevel, &explorationDevaluationPerTimestep, &minAction, &maxAction, &baseOfDimensions);

	resetRobot();

	// Generate all the possible actions the robot could take
	possibleActions = std::vector< std::vector<double> >(pow(baseOfDimensions, actionDimensions), std::vector<double>(actionDimensions, 0));
	std::vector<double> scaleVector(minAction.size());
	if(baseOfDimensions > 1) {
		for (int a = 0; a < actionDimensions; a++) {
			scaleVector[a] = (maxAction[a] - minAction[a]) / ((double)baseOfDimensions - 1);
		}
	} else {
		for (int a = 0; a < actionDimensions; a++) scaleVector[a] = 0;
	}
	std::vector<int> iteratorVector(actionDimensions, 0);
	for (int a = 0; a < pow(baseOfDimensions, actionDimensions); a++) {
		iteratorVector[iteratorVector.size() - 1]++;
		for (int iteratorVectorIndex = iteratorVector.size() - 1; iteratorVectorIndex >= 0; iteratorVectorIndex--) {
			if (iteratorVector[iteratorVectorIndex] >= baseOfDimensions) {
				iteratorVector[iteratorVectorIndex] = 0;
				if (iteratorVectorIndex > 0) iteratorVector[iteratorVectorIndex - 1]++;
			}
		}
		for (int actionDimension = 0; actionDimension < actionDimensions; actionDimension++) {
			possibleActions[a][actionDimension] = iteratorVector[actionDimension] * scaleVector[actionDimension] + minAction[actionDimension];
		}
	}

	for (int a = 0; a < pow(baseOfDimensions, actionDimensions); a++) {
		for (int b = 0; b < actionDimensions; b++) {
			std::cout << possibleActions[a][b] << " ";
		}
		std::cout << "\n";
	}
}

void Robot::run(int trials, int numberOfTimeSteps) {
	std::vector<int> results(trials);
	for (int currentTrial = 0; currentTrial < trials; currentTrial++) {
		resetRobot();

		int a;
		for (a = 0; a < numberOfTimeSteps; a++) {
			//std::this_thread::sleep_for(std::chrono::milliseconds(100));
			currentExplorationLevel *= explorationDevaluationPerTimestep;

			std::vector<double> state = task->getState();

			std::vector<double> action = getAction(learner.chooseBoltzmanAction(state, currentExplorationLevel));
			double reward = task->performAction(action);

			if (task->isTaskDone() == true) break;

			learner.applyReinforcementToLastAction(reward, task->getState());

			// Perform an action a number of times before a reward iteration
			//for (int a = 0; a < 6; a++) {
			//	action = getAction(learner.chooseBoltzmanAction(task->getState(), currentExplorationLevel));
			//	task->performAction(action);
			//}
		}
		std::cout << "iter: " << a << "\n";
		results[currentTrial] = a;
	}

	// Print stats about the number of reward iterations
	WireFitRobot::printStats(std::vector<double>(results.begin(), results.end()));
}

void Robot::performAction() {
	std::cout << "Perform action\n";
	currentExplorationLevel *= explorationDevaluationPerTimestep;
	std::vector<double> state = task->getState();
	std::vector<double> action = getAction(learner.chooseBoltzmanAction(state, currentExplorationLevel));
	task->performAction(action);
}

std::vector<double> Robot::getAction(int action) {
	return possibleActions[action];
}

void Robot::resetRobot() {
	task->reset();

	net::NeuralNet * network = new net::NeuralNet(stateSize, 1, numberOfHiddenLayers, numberOfNeuronsPerHiddenLayer, "sigmoid");
	network->setOutputActivationFunction("simpleLinear");

	double backpropLearningRate = 0.1;
	double backpropMomentumTerm = 0.001;
	double backpropTargetError = 0.1;
	int backpropMaximumIterations = 10000;
	net::Backpropagation backprop = net::Backpropagation(backpropLearningRate, backpropMomentumTerm, backpropTargetError, backpropMaximumIterations);
	backprop.setDerivedOutputActivationFunction("simpleLinear");

	double learningRate = 1;
	double devaluationFactor = 0.0;
	learner = net::QLearn(network, backprop, learningRate, devaluationFactor, pow(baseOfDimensions, actionDimensions));

	currentExplorationLevel = startingExplorationLevel;
}
