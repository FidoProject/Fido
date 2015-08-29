#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>

#include "NeuralNet.h"
#include "QLearn.h"
#include "FidoSim/Simlink.h"

class Robot {
public:
	net::QLearn learner;
	Simlink simulator;
	double boltzmanExplorationLevel, explorationDevaluationPerTimestep;

	Robot();

	std::vector<double> getState();

	double getReward();

	void performAction(int action);

	void run(int numberOfTimeSteps);

};

#endif