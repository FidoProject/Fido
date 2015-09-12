#ifndef __FidoKin__Robby__
#define __FidoKin__Robby__

#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

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

class Robby : public sf::RectangleShape {
public:
    /* Construct a robot of a certain width
     * and height.
     */
    Robby(int width, int height);
    
    /* Set robot motor values and return a
     * gyro sensor TDVect.
     *
     * Should be called every loop.
     * Speed is ~1.
     */
    TDVect go(int motLeft, int motRight, double speed);
    
    /* In testing method which simulates
     * acceleration in the kinematic model.
     *
     * Usage is identical to Robby::go().
     */
    TDVect goAccel(int motLeft, int motRight);
protected:
    // Clock for velocity->distance aproximation.
    sf::Clock moveClock;
    
    int m1Last,m2Last,m1Target,m2Target;
    double lastSpeed;
};

#endif