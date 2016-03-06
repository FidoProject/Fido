#include <iostream>
#include "Hardware.h"
#include "Connection.h"
#include "FlashingLights.h"
#include "../Software/WireFitRobot.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <chrono>
#include <thread>

#define RAD_TO_DEG (180.0/M_PI)

using namespace std;

int main() {
	srand (time(NULL));

	std::cout << "Started\n";

	Connection *connection = new Connection();
	int receiverNum;
	do {
		receiverNum = atoi(connection.getString());
	} while(receiverNum != 5 && receiverNum != 6);
	
	if(receiverNum == 5) {
		Robot robot(new FlashingLights(new Hardware(), connection));
		robot.run(1, 1000);
		while(true) {
			robot.performAction();
		}
		((SoundTask *)robot.task)->hardware->safeClose();
	} else {
		WireFitRobot robot(new FlashingLights(new Hardware(), connection));
		robot.runTrials(1, 1000);
		while(true) {
			robot.performAction();
		}
		((SoundTask *)robot.task)->hardware->safeClose();
	}


	std::cout << "Ended";

	return 0;
}
