#include "StayStill.h"

#include <algorithm>

#include "../../Robot Code/Hardware.h"

StayStill::StayStill(Hardware *hardware_) {
	hardware = hardware_;
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

	*stateSize = 1, *actionDimensions = 2, *numberOfActions = 4, *neuronsPerLayer = 4, *numberOfLayers = 3;
	*beginningExplorationConstant = 0.2, *explorationConstantDevaluation = 1;
	*minAction = { -1, -1 }, *maxAction = { 1, 1 };
	*baseOfDimensions = 6;
}

std::vector<double> StayStill::getState() {
	std::vector<double> state;
	state.push_back(0.5);
	return state;
}

double StayStill::performAction(const std::vector<double> &action) {
	hardware->setMotors((int)(action[0]*100), (int)(action[1]*100));

	sleep(2);

	double loudness = hardware->getLoudness(20);
	std::cout << "Loud: " << loudness << "\n";

	hardware->setMotors(0, 0);
	sleep(2);

	return loudness;
}

bool StayStill::isTaskDone() {
	return false;
}

void StayStill::reset() {
}
