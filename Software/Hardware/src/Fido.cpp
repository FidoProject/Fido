#include <iostream>
#include "Hardware.h"

int main() {
	Hardware fido;

	while(true) {
		std::cout << fido.getVis() << "\n";
		usleep(100000);
	}

	return fido.safeClose();
}
