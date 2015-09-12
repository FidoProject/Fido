#ifndef __FidoKin__Emitter__
#define __FidoKin__Emitter__

#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "Robby.h"

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

class Emitter : public sf::CircleShape {
public:
    // Construct an emitter of a given strength (~50)
    Emitter(int str);
    
    // Set the emitter to appear at a certain location.
    void set(sf::Vector2i mouseLoc);
    
    // Make the emitter go poof.
    void bye();
    
    // Get a simulated TDVect sensor reading.
    TDVect sense(Robby robot);
protected:
    // Strength of the constructed emitter.
    int masterStrength;
    
    // Strength at a certain point in time (either masterStrength or 0)
    int strength;
};

#endif
