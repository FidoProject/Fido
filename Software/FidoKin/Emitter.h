#ifndef __FidoKin__Emitter__
#define __FidoKin__Emitter__

#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

struct TDVect;
class Robby;

class Emitter : public sf::CircleShape {
public:
    // Construct an emitter of a given strength (~50)
    Emitter(int str);
    
    // Set the emitter to appear at a certain location.
    void set(sf::Vector2i mouseLoc);
    
    // Make the emitter go poof.
    void bye();
    
    // Get a simulated TDVect sensor reading.
    //
    // Thresh denotes the distance at which readings
    // level off.
    TDVect sense(Robby robot,int thresh);
protected:
    // Strength of the constructed emitter.
    int masterStrength;
    
    // Strength at a certain point in time (either masterStrength or 0)
    int strength;
};

#endif
