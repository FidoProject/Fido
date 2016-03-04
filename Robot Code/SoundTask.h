#ifndef SoundTask_H
#define SoundTask_H

#include "../Software/Tasks/Task.h"
#include "Hardware.h"
#include "Connection.h"

class SoundTask : public Task {
public:
	Hardware *hardware;
	Connection *connection;
	bool isDone;

	SoundTask(Hardware *hardware_, Connection *connection_);

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
	double getReward();
	bool isTaskDone();
	void reset();
};

#endif
