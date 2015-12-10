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

	*stateSize = 1, *actionDimensions = 1, *numberOfActions = 3, *neuronsPerLayer = 8, *numberOfLayers = 3;
	*beginningExplorationConstant = 0.15, *explorationConstantDevaluation = 1;
	*minAction = { 0 }, *maxAction = { 1 };
	*baseOfDimensions = 6;
}

std::vector<double> StayStill::getState() {
	std::vector<double> state;

	double stateValue;
	std::cout << "state: ";
	std::cin >> stateValue;

	state.push_back(stateValue);
	return state;
}

double StayStill::performAction(const std::vector<double> &action) {
	std::cout << "motors: " << action[0] << "\n";

	double reward;
	std::cin >> reward;

	return reward;
}

bool StayStill::isTaskDone() {
	return false;
}

void StayStill::reset() {
}
