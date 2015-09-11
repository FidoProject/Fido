#include "Simkin.h"

Simkin::Simkin(int robWidth, int robHeight) {
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Fido Kinematic Simulator");
    
    sf::RectangleShape rect(sf::Vector2f(robWidth,robHeight));
    rect.setFillColor(sf::Color(0, 0, 0));
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Q) mLeft=100;
                    else if (event.key.code == sf::Keyboard::A) mLeft=-100;
                    else if (event.key.code == sf::Keyboard::E) mRight=100;
                    else if (event.key.code == sf::Keyboard::D) mRight=-100;
                case sf::Event::KeyReleased:
                    if (event.key.code==sf::Keyboard::Q||event.key.code==sf::Keyboard::A)
                        mLeft=0;
                    else if (event.key.code==sf::Keyboard::E||event.key.code==sf::Keyboard::D)
                        mRight=0;
                default:
                    break;
            }
        } window.clear(sf::Color(255,255,255));
        
        window.draw(rect);

        window.display();
    } return EXIT_SUCCESS;
}