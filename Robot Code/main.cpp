#include <iostream>
#include "Hardware.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define RAD_TO_DEG (180.0/M_PI)

using namespace std;

int main() {
	srand (time(NULL));
	Hardware hardware;
	return hardware.safeClose();
}
