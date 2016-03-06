#include "FlashingLights.h"

#include "../FidoSim/Simlink.h"

#include <iostream>

FlashingLights::FlashingLights(Hardware *hardware_, Connection *connection_) {
	hardware = hardware_;
	connection = connection_;
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

	*stateSize = 1, *actionDimensions = 1, *neuronsPerLayer = 8, *numberOfLayers = 2;
	*beginningExplorationConstant = 0.5, *explorationConstantDevaluation = 1;
	*minAction = { 0 }, *maxAction = { 1 };
	*baseOfDimensions = 2;
}

std::vector<double> FlashingLights::getState() {
	return { (hardware->visVal() - 0.3) / 0.3 };
}

double FlashingLights::performAction(const std::vector<double> &action) {
	simulator->setLED(action[0] * 255, 0, 0, 100);
	std::cout << "Action: " << action[0] << "; ";
	std::cout << "State: " << getState() << "; ";

	float differenceFromState = fabs(action[0] - getState());
	double reward = 1 - 2*differenceFromState;
	std::cout << "Avg: " << avg << "; Reward: " << reward << "; ";

	return reward;
}

bool FlashingLights::isTaskDone() {
	return false;
}

void FlashingLights::reset() {
}