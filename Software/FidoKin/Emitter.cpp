#include "Emitter.h"

#include "Robby.h"
#include "Simlink.h"

Emitter::Emitter(int str):sf::CircleShape(0) {
    setFillColor(sf::Color(255,0,0));
    setOutlineThickness(-3);
    setOutlineColor(sf::Color(0,0,0));
    setOrigin(15,15);
    setPointCount(100);
    masterStrength = str;
    strength = 0;
}

void Emitter::set(sf::Vector2i mouseLoc) {
    setRadius(15);
    strength = masterStrength;
    setPosition(static_cast<sf::Vector2f>(mouseLoc));
}

void Emitter::bye() {
    setRadius(0);
    strength = 0;
}

/// inverse square law
TDVect Emitter::sense(Robby robot,int thresh) {
    int difX = getPosition().x-robot.getPosition().x;
    int difY = getPosition().y-robot.getPosition().y;
    double radius = sqrt(pow(difX,2)+pow((difY),2));
    if (radius<thresh) radius = thresh;
    double mag = strength*80000/(radius*radius);
    
    double theta = atan2(difY,difX)*57.2957795 + 450;
    if (theta>360) theta -= 360;
    theta -= robot.getRotation();
    if (theta<0) theta += 360;
    
    TDVect vect;
    vect.xComp = mag*cos(0.0174532925*theta);
    vect.yComp = mag*sin(0.0174532925*theta);
    vect.zComp = 0;
    
    return vect;
}
