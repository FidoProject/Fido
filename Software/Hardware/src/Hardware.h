#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <cmath>

#include "mraa.hpp"
#include "lib/SFE_LSM9DS0.h"
#include "lib/Sparkfun_TB6612_Edison.h"

struct TDVect {
	// Components of 3d vector
	double xComp, yComp, zComp;

	// Get 3d vector as radius, xy angle, and z angle.
	void getRTP(double *r, double *xy, double *z) {
		*r = sqrt(pow(xComp, 2) + pow(yComp, 2) + pow(zComp, 2));
		*xy = atan2(xComp, yComp);
		*z = acos(zComp / *r);
	}
};

class Hardware {
public:
	Hardware();

	// Returns 3 axis of rotational velocity
	// in degrees per second
	TDVect getGyro();

	// Returns 3 axis of acceleration in Gs
	TDVect getAccel();

	// Returns 3 axis of magnetism in gauss
	TDVect getCompass();

	void setMotors(int motorOne, int motorTwo);
	void chirp(int volume, int frequency);

	int getVis();
	int getMicrophone();
	int getTemperature();

	void setLed(int r, int g, int b, int i);

	virtual ~Hardware();
private:
	LSM9DS0 *imu;
	tb6612 motors;
};

#endif /* HARDWARE_H_ */
