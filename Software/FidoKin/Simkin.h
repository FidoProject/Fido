#ifndef __FidoKin__Simkin__
#define __FidoKin__Simkin__

#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

class Simkin {
public:
    Simkin(int robWidth,int robHeight);
    
private:
    void updateLoop();
    
    // Convert motor values to rotation and translation
    void mToMove(sf::RectangleShape& rect,int mLeft,int mRight);
    
    sf::Clock clock;
};


#endif /* defined(__FidoKin__Simkin__) */
