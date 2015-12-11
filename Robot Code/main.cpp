#include <iostream>
#include "../Software/WireFitRobot.h"
#include "../Software/Tasks/StayStill.h"
#include "BluetoothFido.h"
#include "Hardware.h"

using namespace std;

int main() {
	BluetoothFido blue;
	blue.advertise();
	std::cout << "Done\n";

	//WireFitRobot robot(new StayStill(new Hardware()));
	//robot.runTrials(1, 100);
	//return 0;
	Hardware hardware;
	return hardware.safeClose();
}
