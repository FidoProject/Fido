#ifndef ROBOTN_H
#define ROBOTN_H

#include "QLearn.h"

class Task;

/* Uses the QLearn object paired with the simulator to progressively learn a task through reinforcement learning.
 *
 *
 */
class Robot {
public:
	net::QLearn learner;
	Task *task;
	double startingExplorationLevel, currentExplorationLevel, explorationDevaluationPerTimestep, baseOfDimensions;
	int stateSize, numberOfHiddenLayers, numberOfNeuronsPerHiddenLayer, actionDimensions;
	std::vector<double> minAction, maxAction;
	std::vector< std::vector<double> > possibleActions;

	// Initializes a robot object
	Robot(Task *task_);

	// Runs the robot for a specified number of time steps
	void run(int trials, int numberOfTimeSteps);

	void performAction();

private:
	std::vector<double> getAction(int action);
	void resetRobot();

};

#endif
