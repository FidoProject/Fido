#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "Emitter.h"
#include "Robby.h"
#include "Simlink.h"

/* SIMULATOR GAME THINGY
 *
 * Q makes left motor forwards, A makes left motor
 * backwards, E makes right motor forwards, D makes
 * right motor backwards. Right click adds/moves the
 * emitter, P deletes it.
 */


int main(int argc, char const** argv) {
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Fido Kinematic Simulator");
    
    Robby robot(100,80);
    Emitter emitter(50);
    
    int mLeft, mRight;
    mLeft = mRight = 0;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed) window.close();
        
        /// emitter mouse stuff (copy this)
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            emitter.set(sf::Mouse::getPosition(window));
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            emitter.bye();
        
        /// testing of kinematic model
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) mLeft=100;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) mLeft=-100;
        else mLeft = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) mRight=100;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mRight=-100;
        else mRight = 0;
        
        TDVect emitSense = emitter.sense(robot);
        std::cout << "x:" << emitSense.xComp << ",y:" << emitSense.yComp << "\n";
        
        robot.goAccel(mLeft,mRight);
    
        window.clear(sf::Color(255,255,255));
        window.draw(robot);
        window.draw(emitter);
        window.display();
        sf::sleep(sf::milliseconds(25));
    } return EXIT_SUCCESS;
}
