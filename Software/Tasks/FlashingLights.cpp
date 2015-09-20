#include "FlashingLights.h"

#include "Simlink.h"

FlashingLights::FlashingLights() {};

void FlashingLights::init(Simlink *simulator_) {
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

	*stateSize = 1, *actionDimensions = 1, *numberOfActions = 5, *neuronsPerLayer = 10, *numberOfLayers = 4;
	*beginningExplorationConstant = 0.2, *explorationConstantDevaluation = 1;
	*minAction = { 0 }, *maxAction = { 1 };
	*baseOfDimensions = 6;
}

std::vector<double> FlashingLights::getState() {
	return {(double)simulator->getVis() / 100.0};
}

double FlashingLights::performAction(const std::vector<double> &action) {
	simulator->setLED(action[0] * 255, 0, 0, 100);
	
	if(simulator->getVis() > 50) {
		return action[0];
	} else {
		return 1 - action[0];
	}
}

bool FlashingLights::isTaskDone() {
	return false;
}

void FlashingLights::reset() {
	
}