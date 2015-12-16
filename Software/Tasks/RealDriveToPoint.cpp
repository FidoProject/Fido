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

	*stateSize = 3, *actionDimensions = 2, *numberOfActions = 5, *neuronsPerLayer = 10, *numberOfLayers = 4;
	*beginningExplorationConstant = 0.2, *explorationConstantDevaluation = 1;
	*minAction = { -1, -1 }, *maxAction = { 1, 1 };
	*baseOfDimensions = 6;
}

std::vector<double> RealDriveToPoint::getState() {
	std::vector<double> state;

	double x, y;
	std::cout << "x: ";
	std::cin >> x;
	std::cout << "y: ";
	std::cin >> y;

	double rotation;
	std::cout << "rotation: ";
	std::cin >> rotation;

	state.push_back(x/(x+y));
	state.push_back(y/(x+y));
	state.push_back(rotation / (double)360.0);
	return state;
}

double RealDriveToPoint::performAction(const std::vector<double> &action) {
	hardware->setMotors(action[0]*100, action[1]*100);
	usleep(2500000);
	hardware->setMotors(0, 0);

	double reward;
	std::cout << "reward: ";
	std::cin >> reward;

	return reward;
}

bool RealDriveToPoint::isTaskDone() {
	return false;
}

void RealDriveToPoint::reset() {
}
