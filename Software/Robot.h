#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>

#include "NeuralNet.h"
#include "QLearn.h"
#include "FidoSim/Simlink.h"


/* Uses the QLearn object paired with the simulator to progressively learn a task through reinforcement learning.
 *
 *
 */
class Robot {
public:
	net::QLearn learner;
	Simlink simulator;
	double boltzmanExplorationLevel, explorationDevaluationPerTimestep;

	// Initializes a robot object
	Robot();

	// Runs the robot for a specified number of time steps
	void run(int numberOfTimeSteps);
private:
	// Gets the current state from the simulator
	std::vector<double> getState();

	// Gets a reward from the user
	double getReward();

	// Performs an action using the simulator
	void performAction(int action);

};

#endif