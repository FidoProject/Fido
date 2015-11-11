#include <iostream>
#include "Hardware.h"

int main() {
	Hardware fido;

	fido.setMotors(100,100);
	sleep(1);
	fido.setMotors(-100,-100);
	sleep(1);

	return fido.safeClose();
}
