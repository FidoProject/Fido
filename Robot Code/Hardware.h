#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <iomanip>
#include <cmath>

#include "mraa.hpp"
#include "lib/SFE_LSM9DS0.h"
#include "lib/Sparkfun_TB6612_Edison.h"
#include "lib/SparkFunADS1015.h"
//#include <buzzer.h>

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
	// Initializes IO
	Hardware();

	// Returns 3 axis of rotational velocity
	// in degrees per second
	TDVect getGyro();

	// Returns 3 axis of acceleration in g
	TDVect getAccel();

	// Returns 3 axis of magnetism in Gauss
	TDVect getCompass();

	/* Sets the motors (-100 to 100).
	 *
	 * Zero shorts the motor to brake.
	 */
	void setMotors(int motorOne, int motorTwo);

	/* Buzz for a specific amount of time
	 *
	 * Volume should be 0-1, time in microseconds.
	 */
	void chirp(double volume, int frequency, int time);

	// Returns a visible light intensity value
	// from 0-1
	double getVis();

	// Returns a sound pressure value from 0-1
	double getMicrophone();

	// Returns a loudness value from 0-1
	double getLoudness(int samples);

	// Returns a temperature value from 0-1
	double getTemperature();

	// Returns an infrared light intensity value
	// from 0-1
	double getIR();

	// Set the LED strip to an RGB value (0-1)
	void setLED(double r, double g, double b);

	// Turns off motors and buzzers at end of program.
	int safeClose();

	// Destructs the hardware safely
	virtual ~Hardware();
private:
	LSM9DS0 *imu;
	tb6612 motors;
	mraa::I2c *adc_i2c;
	ads1015 *adc;
	upm::Buzzer *buzz;
	mraa::Pwm* pwmR;
	mraa::Pwm* pwmG;
	mraa::Pwm* pwmB;
};

#endif /* HARDWARE_H_ */
