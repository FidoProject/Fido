#ifndef WIREFITROBOT_H
#define WIREFITROBOT_H

#include "WireFitQLearn.h"
#include "FidoSim/Simlink.h"

class Task;

/* Uses the WireFitQLearn object paired with the simulator to progressively learn a task through reinforcement learning.
 *
 *
 */
class WireFitRobot {
public:
	net::WireFitQLearn learner;
	Task *task;
	double boltzmanExplorationLevel, explorationDevaluationPerTimestep, baseOfDimensions;
	int stateSize, numberOfHiddenLayers, numberOfNeuronsPerHiddenLayer, numberOfActions, actionDimensions;
	std::vector<double> minAction, maxAction;

	// Initializes a robot object
	WireFitRobot(Task *task_);

	// Function for testing the robot on different tasks. Returns a vector containing the number of reward iterations it took to converge on a solution each trial
	std::vector<int> test(int numberOfTimes, int maxIterations);

	// Runs a search of a number of combinations of hyperparameters to determine the best set
	void hyperParameterTest();

	// Print the mean median and mode of the datasetgiven
	static void printStats(std::vector<double> data);
private:
	// Resets the simulator, the learner, and the boltzmanExplorationLevel
	void resetRobot();
};

#endif