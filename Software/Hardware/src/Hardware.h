#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <cmath>
#include <iostream>
#include <unistd.h>

#include "mraa.hpp"
#include "SFE_LSM9DS0.h"

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
};

#endif /* HARDWARE_H_ */
