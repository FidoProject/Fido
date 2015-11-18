#ifndef STAYSTILL_H
#define STAYSTILL_H

#include "Task.h"

class Hardware;

class StayStill : public Task {
public:

	Hardware *hardware;

	const int speed = 3, deltaTime = 20;

	StayStill(Hardware *hardware_);
	
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