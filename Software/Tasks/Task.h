#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <vector>

class Simlink;

// A virtual class that may be subclassed to implement a task to be carried out by WireFitRobot
class Task {
public:
	Simlink *simulator;

	void init(Simlink *simulator_) {
		simulator = simulator_;
	};

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
	virtual double performAction(const std::vector<double> &action) = 0;
	virtual bool isTaskDone() = 0;
};

#endif

