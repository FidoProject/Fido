#include <iostream>
#include "../Software/WireFitRobot.h"
#include "../Software/Tasks/StayStill.h"
#include "Hardware.h"

using namespace std;

int main() {
	WireFitRobot robot(new StayStill(new Hardware()));
	robot.test(1, 100);
	return 0;
	//Hardware hardware;
	//return hardware.safeClose();
}
