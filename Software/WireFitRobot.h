#ifndef WIREFITROBOT_H
#define WIREFITROBOT_H

#include <iostream>
#include <algorithm>

#include "NeuralNet.h"
#include "WireFitQLearn.h"
#include "FidoSim/Simlink.h"
#include "Tasks/Task.h"


/* Uses the WireFitQLearn object paired with the simulator to progressively learn a task through reinforcement learning.
 *
 *
 */
class WireFitRobot {
public:
	net::WireFitQLearn learner;
	Simlink simulator;
	Task *task;
	double boltzmanExplorationLevel, explorationDevaluationPerTimestep, maxDistance, allowableDistance, baseOfDimensions;
	int stateSize, numberOfHiddenLayers, numberOfNeuronsPerHiddenLayer, numberOfActions, actionDimensions;
	std::vector<double> minAction, maxAction;

	// Initializes a robot object
	WireFitRobot(Task *task_);

	// Runs the robot for a specified number of time steps
	void run(int numberOfTimeSteps);

	// Function for testing the robot on different tasks. Returns a vector containing the number of reward iterations it took to converge on a solution each trial
	std::vector<int> test(int numberOfTimes, int maxIterations);

	// Runs a search of a number of combinations of hyperparameters to determine the best set
	void hyperParameterTest();
private:
	// Waits until the user says they have finished inputting the state
	void waitForStateInput();

	// Gets the current state from the simulator
	std::vector<double> getState();

	// Gets a reward from the user
	double getReward();

	// Performs an action using the simulator
	void performAction(const std::vector<double> &action);

	// Resets the simulator, the learner, and the boltzmanExplorationLevel
	void resetRobot();

	// Print the mean median and mode of the datasetgiven
	void printStats(std::vector<int> data);
};

#endif