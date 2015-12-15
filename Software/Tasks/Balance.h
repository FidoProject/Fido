#ifndef BALANCE_H
#define BALANCE_H

#include "Task.h"

class Hardware;

class Balance : public Task {
public:

	Hardware *hardware;
	int iterations, action, layers;
	double explorationConstant;
	double lastState;

	const int speed = 3, deltaTime = 20;

	Balance(Hardware *hardware_,int action_, double explorationConstant_, int layers_);

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

	double deltat;
	unsigned long lastUpdate;

	float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};

	void runKalman();
	void setupKalman();
	void MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
};

#endif
