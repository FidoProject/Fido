#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <cmath>

#include "mraa.hpp"
#include "lib/SFE_LSM9DS0.h"
#include "lib/Sparkfun_TB6612_Edison.h"
#include "SparkFunADS1015.h"

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

	// Sets the motors (-100 to 100).
	// Zero shorts the motor to brake.
	void setMotors(int motorOne, int motorTwo);

	void chirp(int volume, int frequency);

	// Returns a visible light intensity value
	// from 0-2047
	int getVis();

	// Returns a sound pressure value from 0-2047
	int getMicrophone();

	// Returns a temperature value
	int getTemperature();

	// Returns an infrared light intensity value
	// from 0-2047
	int getIR();

	void setLed(int r, int g, int b, int i);

	virtual ~Hardware();
private:
	LSM9DS0 *imu;
	tb6612 motors;
	mraa::I2c *adc_i2c;
	ads1015 *adc;
};

#endif /* HARDWARE_H_ */
