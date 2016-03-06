#include "FlashingLights.h"

#include <iostream>
#include <math.h>

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
	return { (hardware->getVis() - 0.26) / 0.3 };
}

double FlashingLights::performAction(const std::vector<double> &action) {
	std::cout << "Action: " << action[0] << "; ";
	std::cout << "State: " << getState()[0] << "; ";

	//float differenceFromState = fabs(action[0] - getState());
	//double reward = 1 - 2*differenceFromState;

	hardware->setMotors(action[0]*100, action[0]*100);

	if(isDone == false) {
		double reward = connection->getReward();
		if(reward < -1.1 || reward > 1.1) {
			isDone = true;
			return 0;
		}

		return reward;
	}
	return 0;
}

bool FlashingLights::isTaskDone() {
	return isDone;
}

void FlashingLights::reset() {
	isDone = false;
}
