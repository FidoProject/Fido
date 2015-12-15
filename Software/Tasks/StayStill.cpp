#include "StayStill.h"

#include <algorithm>

#include "../../Robot Code/Hardware.h"

StayStill::StayStill(Hardware *hardware_, int action_, double explorationConstant_, int layers_) {
	hardware = hardware_;
	isDone = false;
	iterations = 0;
	action = action_;
	explorationConstant = explorationConstant_;
	layers = layers_;
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

	*stateSize = 1, *actionDimensions = 1, *numberOfActions = 4, *neuronsPerLayer = 10, *numberOfLayers = 4;
	*beginningExplorationConstant = 0.5, *explorationConstantDevaluation = 1;
	*minAction = { 0 }, *maxAction = { 1 };
	*baseOfDimensions = 6;
}

std::vector<double> StayStill::getState() {
	std::vector<double> state;
	state.push_back(0);

	return state;
}

double StayStill::performAction(const std::vector<double> &action) {
	if(action[0] == 0 && iterations > 0) isDone = true;
	iterations++;
	return 1 - (action[0]*2);
}

bool StayStill::isTaskDone() {
	return isDone;
}

void StayStill::reset() {
	isDone = false;
	iterations = 0;
}
