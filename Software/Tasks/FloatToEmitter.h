#ifndef FLOATTOEMITTER_H
#define FLOATTOEMITTER_H

#include "Task.h"

class Simlink;

class FloatToEmitter : public Task {

	Simlink *simulator;
	const double maxDistance = 982, allowableDistance = 110;

	FloatToEmitter();

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
	void performAction(const std::vector<double> &action);
	bool isTaskDone();
};

#endif