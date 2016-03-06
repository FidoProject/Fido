#ifndef FLASHINGLIGHTS_H
#define FLASHINGLIGHTS_H

#include "Task.h"

class Simlink;

class FlashingLights : public Task {
public:
	Simlink *simulator;
	int len, times;
	float avg;
	int lastState;
	std::vector<float> differences;

	FlashingLights(Simlink *simulator_);

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

	bool isDone = false;
};

#endif