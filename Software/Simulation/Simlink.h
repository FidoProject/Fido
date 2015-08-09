#ifndef SIMLINK_H
#define SIMLINK_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace io {
	class Simlink {
	public:
		/* Initialize the simulated IO link.
		 *
		 * Other things.
		 */
		Simlink();

		/* Set the RGB LED to a certain color and intensity.
		 *
		 * Integers r, g, and b are from 0-255, and the intensity is from 0-100.
		 */
		void setLED(int r, int g, int b, int i);

		// Toggle vibrator motor, return new state.
		bool toggleVibrate();

		// End vibration if already vibrating.
		void endVibrate();

		// Return sound intensity from 0-100.
		int getMicrophone();

		// Return IR intensity from 0-100.
		int getIR();

		/* Buzz at a volume and frequency from 0-100 for a duration.
		 *
		 * The frequency is mapped where 0=261Hz (middle C)
		 * and 100 is an octave higher at 523Hz.
		 */
		void buzz(int volume, int frequency, int milisec);

		// Set motor values from -100 to 100 (negative is reverse, 0 is stopped).
		void setMotors(int motorOne, int motorTwo);

		// Stop the motors.
		void stop();

		/* Get the magnitude of the 3D acceleration vector.
		 *
		 * Returns a value in m/s^2.
		 */
		int getForce();

		// Get the current battery level (0-100).
		int getBattery();
	private:
		// Update the "state out" text file based on stored values.
		void putStates();

		// Update stored values based on "state in" text file.
		void getStates();

		// Get three axis of acceleration in m/s^2.
		int* getAccel();

		// Get three axis of rotation velocity in rad/s.
		int* getGyro();

		// LED with values r, g, and b from 0-255.
		struct LED { int r,g,b; };

		// IMU with arrays for accel, gyro, and compass in X,Y,Z format.
		struct IMU { int* accel; int* gyro; int* compass; };

		// Motor container with two motor values from -255 to +255.
		struct Motors { int motorOne, motorTwo; };

		// Buzzer container containing buzzer state.
		struct Buzzer { int volume, frequency; };

		// The state of the onboard pager motor.
		bool vibrating;

		int irVal, micVal, batVal;

		std::ofstream stateOut;
		std::ifstream stateIn;

		LED led;
		Motors motors;
		Buzzer buzzer;
		IMU imu;
	};
}

#endif