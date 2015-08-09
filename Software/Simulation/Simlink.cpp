#include "Simlink.h"
using namespace io;

Simlink::Simlink() {
	std::fstream file0("stateIn.txt"), file1("stateOut.txt");
	if (!file0.good()) {
		std::ofstream maker("stateIn.txt");
		maker.close();
	} if (!file1.good()) {
		std::ofstream maker("stateOut.txt");
		maker.close();
	}
	putStates();
	getStates();
}

void Simlink::putStates() {
	stateOut.open("stateOut.txt",std::ios::trunc);
	if (stateOut.is_open()) {
		stateOut << led.r << "," << led.g << "," << led.b << "\n";
		stateOut << motors.motorOne << "," << motors.motorTwo << "\n";
		stateOut << buzzer.volume << "," << buzzer.frequency << "\n";
		stateOut << vibrating << "/n";
		stateOut.close();
	} else std::cout << "Issue opening state out file";
}

void Simlink::getStates() {
	std::string line;
	stateIn.open("stateIn.txt");
	if (stateIn.is_open()) {
		int i=0;
    	while (getline(stateIn,line)) {
    		std::vector<int> vect;
    		std::stringstream ss(line);
    		int a; 
    		while (ss >> a) {
    			vect.push_back(a);
    			if (ss.peek()==',') ss.ignore();
    		} 
    		switch (i) { /// don't ask about the curlies
    			{case 0: ///accel
    				for (int b=0; b<3; b++)
    					imu.accel[b] = vect[b];
    				break;
    			}{case 1: ///gyro
    				for (int b=0; b<3; b++)
    					imu.compass[b] = vect[b];
    				break;
    			}{case 2: ///compass
    				for (int b=0; b<3; b++)
    					imu.compass[b] = vect[b];
    				break;
    			}{case 3: ///IR sensor
    				irVal = vect[0];
    				break;
    			}{case 4: //microphone
    				micVal = vect[0];
    				break;
    			}{case 5: //battery level
    				batVal = vect[0];
    				break;
    			}
    		} i++;
    	} stateIn.close();
  	}
}

void Simlink::setLED(int r, int g, int b, int i) {
	led.r = r * i/100;
	led.g = g * i/100;
	led.b = b * i/100;
	putStates();
}

bool Simlink::toggleVibrate() {
	vibrating = !vibrating;
	return vibrating;
	putStates();
}

void Simlink::endVibrate() {
	vibrating = false;
	putStates();
}

int Simlink::getMicrophone() {
	getStates();
	return micVal;
}

int Simlink::getIR() {
	getStates();
	return irVal;
}

void Simlink::buzz(int volume, int frequency, int milisec) {
	buzzer.volume = volume;
	buzzer.frequency = frequency * 2.62 + 261;
	/// this only works on certain systems and hogs the thread
	std::this_thread::sleep_for(std::chrono::milliseconds(milisec));
	buzzer.volume = 0;
	putStates();
}

void Simlink::setMotors(int motorOne, int motorTwo) {
	motors.motorOne = motorOne;
	motors.motorTwo = motorTwo;
	putStates();
}

void Simlink::stop() {
	motors.motorOne = motors.motorTwo = 0;
	putStates();
}

int Simlink::getForce() {
	int* curAccel = getAccel();
	int netAccel = sqrt(pow(curAccel[0],2)
					  + pow(curAccel[1],2)
					  + pow(curAccel[2],2));
	return netAccel;
}

int Simlink::getBattery() {
	getStates();
	return batVal;
}

int* Simlink::getAccel() {
	getStates();
	return imu.accel;
}

int* Simlink::getGyro() {
	getStates();
	return imu.gyro;
}