#include "RealDriveToPoint.h"

#include <algorithm>
#include <cmath>

#include "../../Robot Code/Hardware.h"

RealDriveToPoint::RealDriveToPoint(Hardware *hardware_) {
	hardware = hardware_;
};


void RealDriveToPoint::getRobotParameters(int *stateSize,
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
	*beginningExplorationConstant = 0.2, *explorationConstantDevaluation = 1;
	*minAction = { -0.6 }, *maxAction = { 0.6};
	*baseOfDimensions = 2;
}

std::vector<double> RealDriveToPoint::getState() {
	std::vector<double> state;

	return state;
}

double RealDriveToPoint::performAction(const std::vector<double> &action) {
	hardware->setMotors(action[0]*100, action[1]*100);
	usleep(2000000);
	hardware->setMotors(0, 0);
	double reward;
	std::cin >> reward;
	return reward;
}

bool RealDriveToPoint::isTaskDone() {
	return false;
}

void RealDriveToPoint::reset() {
}
