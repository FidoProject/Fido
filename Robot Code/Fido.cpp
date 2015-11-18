#include <iostream>
#include "Hardware.h"
#include <stdio.h>
#include <time.h>
#include "WireFitRobot.h"
#include "StayStill.h"

using namespace std;

int main() {
	WireFitRobot robot(new StayStill(new Hardware()));
	robot.test(20, 5000);
}
