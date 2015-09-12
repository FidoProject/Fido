#ifndef __FidoKin__Robby__
#define __FidoKin__Robby__

#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

class Robby : public sf::RectangleShape {
public:
    /* Construct a robot of a certain width,
     * height, and speed.
     *
     * Speed is ~0-1 range double.
     */
    Robby(int width, int height, double _speed);
    
    /* Set robot motor values.
     *
     * Should be called every loop.
     */
    void go(int motLeft, int motRight);
protected:
    // Clock for velocity->distance aproximation.
    sf::Clock moveClock;
    
    double speed;
};

#endif