#ifndef WIREFITROBOT_H
#define WIREFITROBOT_H

#include <iostream>
#include <algorithm>

#include "NeuralNet.h"
#include "WireFitQLearn.h"
#include "FidoSim/Simlink.h"


/* Uses the WireFitQLearn object paired with the simulator to progressively learn a task through reinforcement learning.
 *
 *
 */
class WireFitRobot {
public:
	net::WireFitQLearn learner;
	Simlink simulator;
	double boltzmanExplorationLevel, explorationDevaluationPerTimestep;

	// Initializes a robot object
	WireFitRobot();

	// Runs the robot for a specified number of time steps
	void run(int numberOfTimeSteps);
private:
	// Waits until the user says they have finished inputting the state
	void waitForStateInput();

	// Gets the current state from the simulator
	std::vector<double> getState();

	// Gets a reward from the user
	double getReward();

	// Performs an action using the simulator
	void performAction(const std::vector<double> &action);

};

#endif