//
//  Emitter.h
//  FidoKin
//
//  Created by Joshua Gruenstein on 9/11/15.
//  Copyright (c) 2015 fido. All rights reserved.
//

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
    Emitter(int str);
    void set(sf::Vector2i mouseLoc);
    void bye();
    TDVect sense(Robby robot);
protected:
    int masterStrength;
    int strength;
};

#endif
