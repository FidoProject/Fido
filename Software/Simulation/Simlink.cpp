#include "Simlink.h"
using namespace io;

Simlink::Simlink() {
	
}

void Simlink::setLED(int r, int g, int b, int i) {
	led.r = r * i/100;
	led.g = g * i/100;
	led.b = b * i/100;
}

bool Simlink::toggleVibrate() {
	vibrating = !vibrating;
	return vibrating;
}

void Simlink::endVibrate() {
	vibrating = false;
}

int Simlink::getMicrophone() {
	return micVal;
}

int Simlink::getIR() {
	return irVal;
}

void Simlink::buzz(int volume, int frequency, int milisec) {
	buzzer.volume = volume;
	buzzer.frequency = frequency * 2.62 + 261;
	/// this only works on certain systems and hogs the thread
	std::this_thread::sleep_for(std::chrono::milliseconds(milisec));
	buzzer.volume = 0;
}

void Simlink::setMotors(int motorOne, int motorTwo) {
	motors.motorOne = motorOne;
	motors.motorTwo = motorTwo;
}

void Simlink::stop() {
	motors.motorOne = motors.motorTwo = 0;
}

int Simlink::getForce() {
	int netAccel = sqrt(pow(getAccel()[0],2)
					  + pow(getAccel()[1],2)
					  + pow(getAccel()[2],2));
	return netAccel;
}

int Simlink::getBattery() {
	return batVal;
}

int* Simlink::getAccel() {
	return imu.accel;
}

int* Simlink::getGyro() {
	return imu.gyro;
}