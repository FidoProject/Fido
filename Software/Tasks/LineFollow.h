#ifndef LINEFOLLOW_H
#define LINEFOLLOW_H

#include "Task.h"

class Simlink;

class LineFollow : public Task {
public:

	Simlink *simulator;

	const int speed = 3, deltaTime = 20;
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
};

#endif