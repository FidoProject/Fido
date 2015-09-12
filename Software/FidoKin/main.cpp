#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "Emitter.h"
#include "Robby.h"

int main(int argc, char const** argv) {
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Fido Kinematic Simulator");
    
    Robby robot(100,80,1);
    
    Emitter emitter(50);
    
    int mLeft, mRight;
    mLeft = mRight = 0;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        } window.clear(sf::Color(255,255,255));
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            emitter.set(sf::Mouse::getPosition(window));
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            emitter.bye();
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) mLeft=100;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) mLeft=-100;
        else mLeft = 0;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) mRight=100;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mRight=-100;
        else mRight = 0;
        
        robot.go(mLeft,mRight);
        
        window.draw(robot);
        window.draw(emitter);
        window.display();
        
        sf::sleep(sf::milliseconds(25));
    } return EXIT_SUCCESS;
}
