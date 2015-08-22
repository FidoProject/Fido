//
//  Simlink.h
//  FidoSim
//
//  Created by Joshua Gruenstein on 8/21/15.
//  Copyright (c) 2015 fido. All rights reserved.
//

#ifndef __FidoSim__Simlink__
#define __FidoSim__Simlink__

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <functional>

class Simlink {
public:
    // Initializes the simulator
    Simlink();
    
    /* Set the RGB LED to a certain color and intensity.
     *
     * Integers r, g, and b are from 0-255, and the intensity is from 0-100.
     */
    void setLED(int r, int g, int b, int i);
    
    // Return sound pressure from 0-100.
    int getMicrophone();
    
    // Return IR intensity from 0-100.
    int getIR();
    
    // Return visible light intensity from 0-100.
    int getVis();
    
    /* Buzz at a volume and frequency from 0-100 for an abritrary duration.
     *
     * The frequency is mapped where 0=261Hz (middle C)
     * and 100 is an octave higher at 523Hz. The duration
     * is currently 200ms.
     */
    void chirp(int volume, int frequency);
    
    /* Set motor values from -100 to 100
     *
     * Positive is forwards, negative is reverse,
     * zero is stopped.
     */
    void setMotors(int motorOne, int motorTwo);
    
    // Stop the motors.
    void stop();
    
    // Get the current battery level (0-100).
    int getBattery();
    
    // Three dimensional vector (used with IMU).
    struct TDVect {
        // Components of 3d vector
        double xComp,yComp,zComp;
        
        // Get 3d vector as radius, xy angle, and z angle.
        void getRTP(double& r,double& xy,double& z) {
            r = sqrt(pow(xComp,2)+pow(yComp,2)+pow(zComp,2));
            xy = atan2(xComp,yComp);
            z = acos(zComp/r);
        }
    };
    
    // Returns the 3D acceleration vector in m/s^2.
    TDVect getAccel();
    
    // Returns the 3D magnetic field vector.
    TDVect getCompass();
    
    /* Returns the 3D rotational velocity vector.
     *
     * The TDVect math here is a little sketchy, but
     * I trust you'll understand what's going on. The
     * components of TDVect are xyz rad/s.
     */
    TDVect getGyro();
    
private:
    void mainLoop(sf::RenderWindow& window,sf::Sprite background);
    
    // LED with values r, g, and b from 0-255.
    struct LED { int r,g,b; };
    
    // IMU with 3D vectors for accel, gyro, and compass.
    struct IMU { TDVect accel; TDVect gyro; TDVect compass; };
    
    // Motor container with two motor values from -255 to +255.
    struct Motors { int motorOne, motorTwo; };
    
    // Sensor values, all ranging from 0-100.
    int irVal, micVal, batVal, visVal;
    
    bool click;
    int cx,cy;
    LED led;
    Motors motors;
    IMU imu;
};

#endif /* defined(__FidoSim__Simlink__) */
