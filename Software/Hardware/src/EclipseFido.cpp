#include <iostream>
#include "Hardware.h"
using namespace std;

int main() {
	Hardware fido;
	cout << "wtqwafdaf";
	fido.setMotors(10, 20);
	usleep(2000);
	return fido.safeClose();
}
