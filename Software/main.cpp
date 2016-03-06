#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <chrono>
#include <thread>

#include "Hardware.h"
#include "Connection.h"
#include "FlashingLights.h"
#include "WireFitRobot.h"
#include "Robot.h"

#define RAD_TO_DEG (180.0/M_PI)

using namespace std;

int main() {
	srand (time(NULL));

	std::cout << "Started\n";

	Connection *connection = new Connection();
	int receiverNum;
	do {
		receiverNum = atoi(connection->getString().c_str());
		std::cout << "Recieved number: " << receiverNum << "\n";
	} while(receiverNum != 5 && receiverNum != 6);
	
	if(receiverNum == 5) {
		std::cout << "Discrete\n";
		Robot robot(new FlashingLights(new Hardware(), connection));
		robot.run(1, 1000);
		std::cout << "Done\n";
		while(true) {
			robot.performAction();
			std::cout << "done with perform\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		((FlashingLights *)robot.task)->hardware->safeClose();
	} else {
		std::cout << "Wirefit\n";
		WireFitRobot robot(new FlashingLights(new Hardware(), connection));
		robot.runTrials(1, 1000);
		std::cout << "Done\n";
		while(true) robot.performAction();
		((FlashingLights *)robot.task)->hardware->safeClose();
	}


	std::cout << "Ended";

	return 0;
}
