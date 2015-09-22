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

int main() {
    srand(time(NULL));  

	WireFitRobot robot(new LineFollow());
	robot.test(500, 5000);

	//Robot robot(new FloatToEmitter());
	//robot.run(100, 3000);
	
	while (true) {

	}

} 