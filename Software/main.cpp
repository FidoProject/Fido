#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>

#include "Neuron.h"
#include "NeuralNet.h"
#include "Backpropagation.h"
#include "QLearn.h"
#include "Robot.h"
#include "WireFitRobot.h"
#include "WireFitQLearn.h"
#include "Tasks/FloatToEmitter.h"
#include "Tasks/DriveToEmitter.h"
#include "Tasks/FlashingLights.h"
#include "Tasks/LineFollow.h"
#include "Tasks/StayStill.h"

int main() {
    srand(time(NULL));  

	int minActions = 3, maxActions = 5, actionIncrement = 1;
	double minExplore = 0.1, maxExplore = 0.7, exploreIncrement = 0.1;
	int minLayers = 3, maxLayers = 5, layerIncrement = 1;

	for (int action = minActions; action < maxActions; action += actionIncrement) {
		for (double explore = minExplore; explore < maxExplore; explore += exploreIncrement) {
			for (int layers = minLayers; layers < maxLayers; layers += layerIncrement) {
				StayStill *still = new StayStill(action, explore, layers);
				WireFitRobot robot(still);
				robot.runTrials(500, 100);
				std::cout << "^^^^^; " << action << ", " << explore << ", " << layers << ".\n";

				delete still;
			}
		}
	}

	//Robot robot(new FloatToEmitter());
	//robot.run(100, 3000);
	
	while (true) {

	}

} 