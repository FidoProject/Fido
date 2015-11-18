#include "StayStill.h"

#include <algorithm>
#include "Hardware.h"

StayStill::StayStill(Hardware *hardware_) {
	hardware = hardware_
};


void StayStill::getRobotParameters(int *stateSize,
						int *actionDimensions,
						int *numberOfActions,
						int *neuronsPerLayer,
						int *numberOfLayers,
						double *beginningExplorationConstant,
						double *explorationConstantDevaluation,
						std::vector<double> *minAction,
						std::vector<double> *maxAction,
						double *baseOfDimensions) {

	*stateSize = 3, *actionDimensions = 2, *numberOfActions = 5, *neuronsPerLayer = 10, *numberOfLayers = 4;
	*beginningExplorationConstant = 0.2, *explorationConstantDevaluation = 1;
	*minAction = { -1, -1 }, *maxAction = { 1, 1 };
	*baseOfDimensions = 6;
}

std::vector<double> StayStill::getState() {
	return {0.5};
}

double StayStill::performAction(const std::vector<double> &action) {
	hardware.setMotors(action[0], action[1]);

	sleep(2);

	return hardware.getLoudness(20);
	
}

bool StayStill::isTaskDone() {
	return false;
}

void StayStill::reset() {
}