#include <iostream>
#include "Hardware.h"
#include "Connection.h"
#include "SoundTask.h"
#include "../Software/WireFitRobot.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define RAD_TO_DEG (180.0/M_PI)

using namespace std;

int main() {
	srand (time(NULL));

	std::cout << "Started\n";

	WireFitRobot robot(new SoundTask(new Hardware(), new Connection()));
	robot.runTrials(1, 1000);

	std::cout << "Ended";

	((SoundTask *)robot.task)->hardware->safeClose();

	return 0;
}
