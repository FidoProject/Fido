#include <iostream>
#include "Hardware.h"

int main() {
	Hardware fido;

	while(true) std::cout << fido.getLoudness(20) << "\n";

	return fido.safeClose();
}
