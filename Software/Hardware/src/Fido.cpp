#include <iostream>
#include "Hardware.h"

int main() {
	Hardware fido;


	while (true) {
		double fun = fido.getAccel().zComp;
		std::cout << fun << "\n";
		sleep(1);
	}
	return 0;
}
