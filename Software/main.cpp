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

	FlashingLights *still = new FlashingLights(new Simlink());
	Robot robot(still);
	std::cout << "start\n";
	robot.run(100, 100);
	std::cout << "end\n";

	delete still;

}