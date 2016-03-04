#ifndef SoundTask_H
#define SoundTask_H

#include "../Software/Tasks/Task.h"

class SoundTask : public Task {
public:
	Hardware *hardware;


	SoundTask(Hardware *hardware_);

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
