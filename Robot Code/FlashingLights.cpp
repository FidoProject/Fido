#include "FlashingLights.h"

#include "../FidoSim/Simlink.h"

#include <iostream>

FlashingLights::FlashingLights(Simlink *simulator_) {
	simulator = simulator_;
};

void FlashingLights::getRobotParameters(int *stateSize,
						int *actionDimensions,
						int *numberOfActions,
						int *neuronsPerLayer,
						int *numberOfLayers,
						double *beginningExplorationConstant,
						double *explorationConstantDevaluation,
						std::vector<double> *minAction,
						std::vector<double> *maxAction,
						double *baseOfDimensions) {

	*stateSize = 1, *actionDimensions = 1, *neuronsPerLayer = 8, *numberOfLayers = 2;
	*beginningExplorationConstant = 0.5, *explorationConstantDevaluation = 1;
	*minAction = { 0 }, *maxAction = { 1 };
	*baseOfDimensions = 2;
}

std::vector<double> FlashingLights::getState() {
	return { lastState };
}

double FlashingLights::performAction(const std::vector<double> &action) {
	simulator->setLED(action[0] * 255, 0, 0, 100);
	std::cout << "Action: " << action[0] << "; ";
	std::cout << "State: " << lastState << "; ";

	float differenceFromState = fabs(action[0] - lastState);

	if(differences.size() < 5) {
		differences.push_back(differenceFromState);
	} else {
		differences.erase(differences.begin());
		differences.push_back(differenceFromState);

		avg = 0;
		for(int a = 0; a < differences.size(); a++) avg += differences[a];
		avg /= (float)differences.size();

		if(avg <= 0.21) {
			isDone = true;
		}
	}

	double reward = 1 - 2*differenceFromState;
	std::cout << "Avg: " << avg << "; Reward: " << reward << "; ";

	lastState = int(lastState == 0);

	return reward;
}

bool FlashingLights::isTaskDone() {
	return isDone;
}

void FlashingLights::reset() {
	simulator->visVal = ((double)rand() / (double)RAND_MAX) * 100;
	isDone = false;
	avg = 0;
	differences = std::vector<float>(0);
	lastState = 0;
	times = 0;
}