#include <iostream>
#include "../Software/WireFitRobot.h"
#include "../Software/Tasks/StayStill.h"
#include "BluetoothFido.h"
#include "Hardware.h"
#include <stdlib.h>
#include <time.h>

using namespace std;

int main() {
	srand (time(NULL));
	//BluetoothFido blue;
	//blue.advertise();
	//std::cout << "Done\n";
	/*int minActions = 3, maxActions = 5, actionIncrement = 1;
	double minExplore = 0.1, maxExplore = 0.7, exploreIncrement = 0.1;
	int minLayers = 3, maxLayers = 5, layerIncrement = 1;

	for(int action = minActions; action < maxActions; action += actionIncrement) {
		for(double explore = minExplore; explore < maxExplore; explore += exploreIncrement) {
			for(int layers = minLayers; layers < maxLayers; layers += layerIncrement) {
				Hardware *hardware = new Hardware();
				StayStill *still = new StayStill(hardware, action, explore, layers);
				WireFitRobot robot(still);
				robot.runTrials(500, 100);
				std::cout << "^^^^^; " << action << ", " << explore << ", " << layers << ".\n";

				delete still;
				delete hardware;
			}
		}
	}*/
	Hardware hardware;
	while(true) {
		TDVect gyro = hardware.getAccel();
		std::cout << gyro.xComp << ", " << gyro.yComp << ", " << gyro.zComp << "\n" << std::flush;
		//std::cout << "y: " << atan(gyro.yComp / sqrt(pow(gyro.xComp, 2) + pow(gyro.zComp, 2))) << "\n" << std::flush;
		usleep(100000);
	}
	return 0;
	//Hardware hardware;
	//return hardware.safeClose();
}
