#include "LineFollow.h"

#include <algorithm>
#include "Simlink.h"

LineFollow::LineFollow() {};

void LineFollow::init(Simlink *simulator_) {
	simulator = simulator_;
};

void LineFollow::getRobotParameters(int *stateSize,
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

std::vector<double> LineFollow::getState() {
	double distance = distanceFromLine(simulator->robot.getPosition());
	double leftDistance = distanceFromLine(simulator->robot.getPoint(0));
	double rightDistance = distanceFromLine(simulator->robot.getPoint(1));
	
	if (leftDistance < rightDistance) return {1, 0};
	else return { 0, 1 };
}

double LineFollow::performAction(const std::vector<double> &action) {
	double previousDistance = distanceFromLine(simulator->robot.getPosition());

	simulator->robot.go(action[0] * 100, action[1] * 100, speed, deltaTime);

	return (double)(previousDistance - distanceFromLine(simulator->robot.getPosition())) / 1.415;
	
}

bool LineFollow::isTaskDone() {
	return false;
}

void LineFollow::reset() {
	simulator->placeRobotInRandomPosition();

	p1 = simulator->robot.getPosition();
	p2 = {5.0*cos(simulator->robot.getRotation()*3.14159265/180.0), 5.0*sin(simulator->robot.getRotation()*3.14159265/180.0)};

}

double LineFollow::distanceFromLine(sf::Vector2f p) {
	return abs((p2.y-p1.y)*p.x - (p2.x-p1.x)*p.y + p2.x*p1.y - p2.y*p1.x) / sqrt(pow(p2.y - p1.y, 2) + pow(p2.x - p1.x, 2));
}