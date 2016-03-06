#ifndef FLASHINGLIGHTS_H
#define FLASHINGLIGHTS_H

#include "Task.h"

class Simlink;

class FlashingLights : public Task {
public:
	Hardware *hardware;
	Connection *connection;

	FlashingLights(Hardware *hardware_, Connection *connection_);

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