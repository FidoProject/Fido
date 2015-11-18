#include "FlashingLights.h"

#include "Simlink.h"

void FlashingLights(Simlink *simulator_) {
	simulator = simulator_;
};

void FlashingLights::getRobotParameters(int *stateSize,
						int *actionDimensions,
						int *numberOfActions,
						int *neuronsPerLayer,
						int *numberOfLayers,
						double *beginningExplorationConstant,
						double *explorationConstantDevaluation,
						std::vector<double> *minAction,
						std::vector<double> *maxAction,
						double *baseOfDimensions) {

	*stateSize = 1, *actionDimensions = 1, *numberOfActions = 4, *neuronsPerLayer = 10, *numberOfLayers = 3;
	*beginningExplorationConstant = 0.2, *explorationConstantDevaluation = 1;
	*minAction = { 0 }, *maxAction = { 1 };
	*baseOfDimensions = 6;
}

std::vector<double> FlashingLights::getState() {
	simulator->visVal = ((double)rand() / (double)RAND_MAX) * 100;
	return{ simulator->getVis() / 100.0 };
}

double FlashingLights::performAction(const std::vector<double> &action) {
	simulator->setLED(action[0] * 255, 0, 0, 100);

	if (abs(action[0] - simulator->getVis() / 100.0) < 0.02) isDone = true;
	
	return 1 - abs(action[0] - simulator->getVis() / 100.0);
}

bool FlashingLights::isTaskDone() {
	return isDone;
}

void FlashingLights::reset() {
	simulator->visVal = ((double)rand() / (double)RAND_MAX) * 100;
	isDone = false;
}