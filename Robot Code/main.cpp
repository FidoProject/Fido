#include <iostream>
#include "../Software/WireFitRobot.h"
#include "../Software/Tasks/StayStill.h"
#include "BluetoothFido.h"
#include "Hardware.h"
#include "../Software/Tasks/Balance.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define RAD_TO_DEG (180.0/M_PI)

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
	/*Hardware hardware;
	while(true) {
		TDVect gyro = hardware.getAccel();
		//std::cout << gyro.xComp << ", " << gyro.yComp << ", " << gyro.zComp << "\n" << std::flush;
		//double pitch = atan(-gyro.xComp / sqrt(gyro.yComp * gyro.yComp + gyro.zComp * gyro.zComp)) * RAD_TO_DEG;
		double pitch = atan2(-gyro.xComp, gyro.zComp) * RAD_TO_DEG;
		std::cout << "y: " << pitch << "\n" << std::flush;
		usleep(100000);
	}*/
	Hardware *hardware = new Hardware();
	//hardware->safeClose();
	Balance *balance = new Balance(hardware, 0, 0, 0);
	//WireFitRobot robot(balance);
	//robot.runTrials(1, 1000000);
	balance->setupKalman();
	while(true) {
		balance->runKalman();
		usleep(50000);
	}
	return 0;
	//Hardware hardware;
	//return hardware.safeClose();
}
