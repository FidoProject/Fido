#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>

#include "NeuralNet.h"
#include "QLearn.h"
#include "FidoSim/Simlink.h"
#include "Tasks/Task.h"

/* Uses the QLearn object paired with the simulator to progressively learn a task through reinforcement learning.
 *
 *
 */
class Robot {
public:
	net::QLearn learner;
	Simlink simulator;
	Task *task;
	double boltzmanExplorationLevel, explorationDevaluationPerTimestep, baseOfDimensions;
	int stateSize, numberOfHiddenLayers, numberOfNeuronsPerHiddenLayer, actionDimensions;
	std::vector<double> minAction, maxAction;

	// Initializes a robot object
	Robot(Task *task_);

	// Runs the robot for a specified number of time steps
	void run(int numberOfTimeSteps);

private:
	std::vector<double> getAction(int action);

};

#endif