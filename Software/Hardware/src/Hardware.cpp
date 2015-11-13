#include "Hardware.h"

#define BUZZ_PIN 14
#define LED_R_PIN 20
#define LED_G_PIN 0
#define LED_B_PIN 21

Hardware::Hardware() {
	/// init IMU
	imu = new LSM9DS0(0x6B, 0x1D);
	imu->begin();

	/// init motors
	motors.standby(false);

	/// init adc
    adc_i2c = new mraa::I2c(1);
    adc = new ads1015(adc_i2c, 0x48);
    adc->setRange(_4_096V);

    /// init buzzer
    buzz = new upm::Buzzer(BUZZ_PIN);

    /// init RGB strip
    pwmR = new mraa::Pwm(LED_R_PIN);
    pwmG = new mraa::Pwm(LED_G_PIN);
	pwmB = new mraa::Pwm(LED_B_PIN);
	pwmR->enable(true);
	pwmG->enable(true);
	pwmB->enable(true);
}

TDVect Hardware::getGyro() {
	TDVect returnVect;
	imu->readGyro();
	returnVect.xComp = imu->calcGyro(imu->gx);
	returnVect.yComp = imu->calcGyro(imu->gy);
	returnVect.zComp = imu->calcGyro(imu->gz);
	return returnVect;
}

TDVect Hardware::getAccel() {
	TDVect returnVect;
	imu->readAccel();
	returnVect.xComp = imu->calcAccel(imu->ax);
	returnVect.yComp = imu->calcAccel(imu->ax);
	returnVect.zComp = imu->calcAccel(imu->ax);
	return returnVect;
}

TDVect Hardware::getCompass() {
	TDVect returnVect;
	imu->readMag();
	returnVect.xComp = imu->calcMag(imu->mx);
	returnVect.yComp = imu->calcMag(imu->my);
	returnVect.zComp = imu->calcMag(imu->mz);
	return returnVect;
}

void Hardware::setMotors(int motorOne, int motorTwo) {
	motors.shortBrake(motorOne==0,motorTwo==0);
	double scaleOne = motorOne/-100.0;
	double scaleTwo = motorTwo/-100.0;
	motors.diffDrive(scaleOne,scaleTwo);
}

void Hardware::chirp(double volume, int frequency, int time) {
	buzz->setVolume(volume);
	buzz->playSound(frequency,time);
}

double Hardware::getVis() {
	return adc->getRawResult(0)/2047.0;
}

double Hardware::getMicrophone() {
	return adc->getRawResult(2)/2047.0;
}

double Hardware::getLoudness(int samples) {
	double x = 0;
	for (int i=0; i<samples; i++) {
		double y = getMicrophone();
		x = (y>x)?y:x;
	} return x;
}

double Hardware::getIR() {
	return adc->getRawResult(1)/2047.0;
}

void Hardware::setLed(double r, double g, double b) {
	pwmR->write(r);
	pwmG->write(g);
	pwmB->write(b);
}

double Hardware::getTemperature() {
	imu->readTemp();
	return imu->temperature/4096.0;
}

int Hardware::safeClose() {
	for (int i=0; i<3; i++) {
		setMotors(0,0);
		buzz->stopSound();
		setLed(0,0,0);
	} return 1;
}

Hardware::~Hardware() {
	safeClose();

	delete buzz;
	delete adc_i2c;
	delete adc;
	delete imu;
	delete pwmR;
	delete pwmG;
	delete pwmB;
}

