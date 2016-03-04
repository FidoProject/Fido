#ifndef TASK_H
#define TASK_H

#include <vector>

class Simlink;

// A virtual class that may be subclassed to implement a task to be carried out by WireFitRobot
class Task {
public:
	virtual void getRobotParameters(int *stateSize, 
									int *actionDimensions, 
									int *numberOfActions, 
									int *neuronsPerLayer, 
									int *numberOfLayers,
									double *beginningExplorationConstant,
									double *explorationConstantDevaluation,
									std::vector<double> *minAction,
									std::vector<double> *maxAction,
									double *baseOfDimensions) = 0;
	virtual std::vector<double> getState() = 0;
	virtual void performAction(const std::vector<double> &action) = 0;
	virtual double getReward();
	virtual bool isTaskDone() = 0;
	virtual void reset() = 0;
};

#endif

