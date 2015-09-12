#ifndef __FidoKin__Robby__
#define __FidoKin__Robby__

#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

class Robby : public sf::RectangleShape {
public:
    Robby(int width, int height, double _speed);
    void go(int motLeft, int motRight);
protected:
    sf::Clock moveClock;
    double speed;
};

#endif