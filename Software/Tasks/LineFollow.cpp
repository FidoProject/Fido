#include "LineFollow.h"

#include <algorithm>
#include "Simlink.h"

void LineFollow(Simlink *simulator_) {
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

	*stateSize = 3, *actionDimensions = 2, *numberOfActions = 4, *neuronsPerLayer = 10, *numberOfLayers = 4;
	*beginningExplorationConstant = 0.2, *explorationConstantDevaluation = 1;
	*minAction = { -1, -1 }, *maxAction = { 1, 1 };
	*baseOfDimensions = 11;
}

std::vector<double> LineFollow::getState() {
	double distance = distanceFromLine(simulator->robot.getPosition());
	double leftDistance = distanceFromLine(simulator->robot.getPoint(0));
	double rightDistance = distanceFromLine(simulator->robot.getPoint(1));

	sf::Vector2f point = getFootOfPerpendicular(simulator->robot.getPosition(), p1, p2);
	double x = point.x;
	double y = point.y;

	return {x / (abs(x) + abs(y)), y / (abs(x) + abs(y)), (double)simulator->robot.getRotation() / 360.0 };
	
	//if (leftDistance < rightDistance) return {1, 0, (double)simulator->robot.getRotation() / 360.0 };
	//else return { 0, 1, (double)simulator->robot.getRotation() / 360.0 };
}

double LineFollow::performAction(const std::vector<double> &action) {
	double previousDistance = distanceFromLine(simulator->robot.getPosition());

	simulator->robot.go(action[0] * 100, action[1] * 100, speed, deltaTime);

	double distance = distanceFromLine(simulator->robot.getPosition());

	if (distance < allowableDistance)  timesOn++;
	else timesOn = 0;

	return (double)(previousDistance - distance) / 1.5;

	//if(distance > allowableDistance) return (double)(previousDistance - distance) / 1.5;
	//else return (double)(previousDistance - distance) / 0.5;
	
}

bool LineFollow::isTaskDone() {
	double distance = distanceFromLine(simulator->robot.getPosition());
	///std::cout << "distance: " << distance << "\n";

	if (timesOn > 10) return true;
	else return false;
}

void LineFollow::reset() {
	simulator->placeRobotInRandomPosition();

	p1 = simulator->robot.getPosition();
	p2 = {p1.x + (float)(500.0*cos((simulator->robot.getRotation()+90)*3.14159265/180.0)), p1.y + (float)(500.0*sin((simulator->robot.getRotation()+40)*3.14159265/180.0))};

	do {
		simulator->placeRobotInRandomPosition();
	} while(distanceFromLine(simulator->robot.getPosition()) < allowableDistance);

	simulator->drawLine(p1, p2);

}

double LineFollow::distanceFromLine(sf::Vector2f p) {
	return abs((p2.y-p1.y)*p.x - (p2.x-p1.x)*p.y + p2.x*p1.y - p2.y*p1.x) / sqrt(pow(p2.y - p1.y, 2) + pow(p2.x - p1.x, 2));
}