#include "Robot.h"

#include <iostream>
#include <algorithm>
#include <time.h>
#include <chrono>
#include <thread>

#include "LSInterpolator.h"
#include "Task.h"
#include "Learner.h"
#include "NeuralNet.h"

using namespace rl;

Robot::Robot(Task *task_, Learner *learner_, double startingExplorationLevel_, double explorationDevaluationPerTimestep_) {
	task = task_;
	learner = learner_;
	startingExplorationLevel = startingExplorationLevel_;
	explorationDevaluationPerTimestep = explorationDevaluationPerTimestep_;

	resetRobot();
}

void Robot::run(int trials, int numberOfTimeSteps) {
	for (int currentTrial = 0; currentTrial < trials; currentTrial++) {
		resetRobot();

		int rewardIteration;
		for (rewardIteration = 0; rewardIteration < numberOfTimeSteps; rewardIteration++) {
			performAction();
			if (task->isTaskDone() == true) break;
			learner->applyReinforcementToLastAction(task->getRewardForLastStateAction(), task->getState());
		}
	}
}

void Robot::performAction() {
	currentExplorationLevel *= explorationDevaluationPerTimestep;
	std::vector<double> state = task->getState();
	std::vector<double> action = learner->chooseBoltzmanAction(state, currentExplorationLevel);
	task->performAction(action);
}

void Robot::resetRobot() {
	task->reset();
	learner->reset();

	currentExplorationLevel = startingExplorationLevel;
}