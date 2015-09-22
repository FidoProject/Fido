#include "MakeShape.h"

#include <algorithm>
#include "Simlink.h"


MakeShape::MakeShape() {};

void MakeShape::init(Simlink *simulator_) {
	simulator = simulator_;
};

void MakeShape::getRobotParameters(int *stateSize,
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

std::vector<double> MakeShape::getState() {
	
}

double MakeShape::performAction(const std::vector<double> &action) {
	simulator->robot.go(action[0] * 100, action[1] * 100, speed, deltaTime);

	return 0;
	
}

bool MakeShape::isTaskDone() {
	return false;
}

void MakeShape::reset() {
	simulator->placeRobotInRandomPosition();
}