#ifndef MAKESHAPE_H
#define MAKESHAPE_H

#include "Task.h"

class Simlink;

class MakeShape : public Task {
public:

	Simlink *simulator;

	const int speed = 3, deltaTime = 20;

	MakeShape();

	void init(Simlink *simulator_);
	void getRobotParameters(int *stateSize, 
							int *actionDimensions, 
							int *numberOfActions, 
							int *neuronsPerLayer, 
							int *numberOfLayers,
							double *beginningExplorationConstant,
							double *explorationConstantDevaluation,
							std::vector<double> *minAction,
							std::vector<double> *maxAction,
							double *baseOfDimensions);
	std::vector<double> getState();
	double performAction(const std::vector<double> &action);
	bool isTaskDone();
	void reset();
};

#endif