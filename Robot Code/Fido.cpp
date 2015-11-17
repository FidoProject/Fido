#include <iostream>
#include "Hardware.h"
#include <stdio.h>
#include <time.h>
using namespace std;

int main() {
	Hardware fido;

	std::cout << "\n\n\n\n\nCan you see me\n\n\n\n";
	fido.setMotors(0, 50);
	fido.setLed(0, 1, 0);
	sleep(2);
	fido.setMotors(50, 0);
	fido.setLed(1, 0, 0);
	sleep(2);
	fido.setMotors(50, 50);
	fido.setLed(0, 0, 1);
	sleep(2);
	return fido.safeClose();
}
