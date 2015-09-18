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


int main() {
    srand(time(NULL));
    
	//Simlink link;

	WireFitRobot robot;
	robot.test(50, 3000);

	while (true) {

	}

} 