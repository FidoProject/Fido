#ifndef DRIVER_H
#define DRIVER_H

#include <iostream>

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

class Driver {
public:
	/// ACTIONS

	/* Sets the motors (-100 to 100).
	 *
	 * Zero shorts the motor to brake.
	 */
	virtual void setMotors(int motorOne, int motorTwo) = 0;

	// Set the LED strip to an RGB value (0-1)
	virtual void setLED(double r, double g, double b) = 0;

	/* Buzz at a volume and frequency from 0-100 for a set duration.
	 *
	 * The frequency is mapped where 0=261Hz (middle C)
	 * and 100 is an octave higher at 523Hz.
	 */
	virtual void chirp(double volume, int frequency, int time) = 0;

	/// SENSORS

	// Returns a visible light intensity value
	// from 0-1
	virtual double getVis() = 0;

	// Returns a sound pressure value from 0-1
	virtual double getMicrophone() = 0;

	// Returns a loudness value from 0-1
	virtual double getLoudness(int samples) = 0;

	// Returns a temperature value from 0-1
	virtual double getTemperature() = 0;

	// Returns an infrared light intensity value
	// from 0-1
	virtual double getIR() = 0;

	 // Returns the 3D acceleration vector in m/s^2.
	virtual TDVect getAccel() = 0;
	
	// Returns the 3D magnetic field vector.
	virtual TDVect getCompass() = 0;
	
	/* Returns the 3D rotational velocity vector.
	 *
	 * The TDVect math here is a little sketchy, but
	 * I trust you'll understand what's going on. The
	 * components of TDVect are xyz rad/s.
	 */
	virtual TDVect getGyro() = 0;
};

#endif