#include "FloatToEmitter.h"

#include "Simlink.h"

FloatToEmitter::FloatToEmitter() {}

void FloatToEmitter::getRobotParameters(int *stateSize,
						int *actionDimensions,
						int *numberOfActions,
						int *neuronsPerLayer,
						int *numberOfLayers,
						double *beginningExplorationConstant,
						double *explorationConstantDevaluation,
						std::vector<double> *minAction,
						std::vector<double> *maxAction,
						double *baseOfDimensions) {



}

std::vector<double> FloatToEmitter::getState() {
	double x, y;
	simulator->getRobotDisplacementFromEmitter(&x, &y);

	return {x / maxDistance, y / maxDistance};
}

void FloatToEmitter::performAction(const std::vector<double> &action) {
	sf::Vector2f previousRobotPosition = simulator->robot.getPosition();

	simulator->robot.setPosition(simulator->robot.getPosition().x + action[0] * 10, simulator->robot.getPosition().y + action[1] * 10);

	sf::Vector2f newRobotPosition = simulator->robot.getPosition();
	if (newRobotPosition.x < 500 + simulator->robot.getGlobalBounds().height / 2
		|| newRobotPosition.x > 1200 - simulator->robot.getGlobalBounds().height / 2
		|| newRobotPosition.y < 0 + simulator->robot.getGlobalBounds().height / 2
		|| newRobotPosition.y > 595 - simulator->robot.getGlobalBounds().height / 2) {
		
		simulator->robot.setPosition(previousRobotPosition);
	}
}

bool FloatToEmitter::isTaskDone() {
	if (simulator->getDistanceOfRobotFromEmitter() < allowableDistance) {
		return true;
	} else {
		return false;
	}
}
 