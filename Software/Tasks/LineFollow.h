#ifndef LINEFOLLOW_H
#define LINEFOLLOW_H

#include "Task.h"
#include <SFML/Graphics.hpp>

class Simlink;

class LineFollow : public Task {
public:

	Simlink *simulator;

	const double allowableDistance = 60;
	const int speed = 2, deltaTime = 20;
	sf::Vector2f p1, p2;

	LineFollow();

	void init(Simlink *simulator_);
	void getRobotParameters(int *stateSize, 
							int *actionDimensions, 
							int *numberOfActions, 
							int *neuronsPerLayer, 
							int *numberOfLayers,
							double *beginningExplorationConstant,
							double *explorationConstantDevaluation,
							std::vector<double> *minAction,
							std::vector<double> *maxAction,
							double *baseOfDimensions);
	std::vector<double> getState();
	double performAction(const std::vector<double> &action);
	bool isTaskDone();
	void reset();

	double distanceFromLine(sf::Vector2f p);

	sf::Vector2f getFootOfPerpendicular(sf::Vector2f p, sf::Vector2f p1, sf::Vector2f p2) {
		float lineSlope = (p2.y- p1.y) / (p2.x - p1.x);
		float lineIntercept = p1.y - lineSlope*p1.x;
		float perpendicularSlope = -1.0 / lineSlope;
		float perpendicularIntercept = p.y - perpendicularSlope*p.x;
		float intersectionX = (lineIntercept - perpendicularIntercept) / (perpendicularSlope - lineSlope);
		float intersectionY = lineSlope*intersectionX + lineIntercept;
		return{ intersectionX, intersectionY };
	}

	int timesOn = 0;
};

#endif