#include "DriveToEmitter.h"

#include "../FidoSim/Simlink.h"
#include <iostream>

DriveToEmitter::DriveToEmitter(Simlink *simulator_) {
	simulator = simulator_;
};

void DriveToEmitter::getRobotParameters(int *stateSize,
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
	*beginningExplorationConstant = 0.5, *explorationConstantDevaluation = 1;
	*minAction = { -1, -1 }, *maxAction = { 1, 1 };
	*baseOfDimensions = 6;
}

std::vector<double> DriveToEmitter::getState() {
	double x, y;
	simulator->getRobotDisplacementFromEmitter(&x, &y);

	return {x / (abs(x) + abs(y)), y / (abs(x) + abs(y)), (double)simulator->robot.getRotation() / 360.0};
}

double DriveToEmitter::performAction(const std::vector<double> &action) {
	sf::Vector2f previousRobotPosition = simulator->robot.getPosition();
	double previousDistance = simulator->getDistanceOfRobotFromEmitter();

	simulator->robot.go(action[0] * 100, action[1] * 100, speed, deltaTime);

	sf::Vector2f newRobotPosition = simulator->robot.getPosition();
	if (newRobotPosition.x < 500 + simulator->robot.getGlobalBounds().height / 2
		|| newRobotPosition.x > 1200 - simulator->robot.getGlobalBounds().height / 2
		|| newRobotPosition.y < 0 + simulator->robot.getGlobalBounds().height / 2
		|| newRobotPosition.y > 595 - simulator->robot.getGlobalBounds().height / 2) {
		
		///simulator->robot.setPosition(previousRobotPosition);
	}

	if (simulator->robot.getPosition() == previousRobotPosition) turnsStill++;
	else turnsStill = 0;
	if (simulator->getDistanceOfRobotFromEmitter() > previousDistance) turnsAway++;
	else turnsAway = 0;

	return (double)(previousDistance - simulator->getDistanceOfRobotFromEmitter()) / 1.415;

	///return (1 - (turnsStill*0.06 + turnsAway*0.1)) - (simulator->getDistanceOfRobotFromEmitter() / maxDistance);
	
}

bool DriveToEmitter::isTaskDone() {
	if (simulator->getDistanceOfRobotFromEmitter() < 80) {
		return true;
	} else {
		return false;
	}
}

void DriveToEmitter::reset() {
	do {
		simulator->placeEmitterInRandomPosition();
		simulator->placeRobotInRandomPosition();
	} while(simulator->getDistanceOfRobotFromEmitter() < 80);


}