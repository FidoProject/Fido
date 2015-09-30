#ifndef DRIVETOEMITTER_H
#define DRIVETOEMITTER_H

#include "Task.h"

class Simlink;

class DriveToEmitter : public Task {
public:
	Simlink *simulator;
	const double maxDistance = 3000, allowableDistance = 110;
	const int speed = 3, deltaTime = 20;
	double turnsStill = 0, turnsAway = 0;

	DriveToEmitter();

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