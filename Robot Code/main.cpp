#include <iostream>
#include "Hardware.h"
#include "Connection.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define RAD_TO_DEG (180.0/M_PI)

using namespace std;

int main() {
	srand (time(NULL));

	std::cout << "Started\n";

	Hardware hardware;
	Connection connection;

	std::cout << "Connected\n";
	std::cout << "Reward: " << connection.getReward() << "\n";

	connection.disconnect();

	return hardware.safeClose();
}
