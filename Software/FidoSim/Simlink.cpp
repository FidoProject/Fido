#include "Simlink.h"

Simlink::Simlink() {
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Fido Simulator");

    
    sf::Texture texture;
    if (!texture.loadFromFile("background.png")) {
        exit(EXIT_FAILURE);
    } sf::Sprite background(texture);
    
    sf::Font font;
    if (!font.loadFromFile("sansation.ttf")) {
		exit(EXIT_FAILURE);
    } sf::Text text("Hello SFML", font, 50);
    text.setColor(sf::Color::Black);
    
    sf::Music music;
    if (!music.openFromFile("nice_music.ogg")) {
		exit(EXIT_FAILURE);
    } music.play();
    
    while (window.isOpen()) {
        mainLoop(window,background);
    }
    
}

void Simlink::mainLoop(sf::RenderWindow& window,sf::Sprite background) {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    click = true;
                    cx = event.mouseButton.x;
                    cy = event.mouseButton.y;
                } break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left)
                    click = false;
                break;
            case sf::Event::MouseMoved:
                if (click) {
                    cx = event.mouseMove.x;
                    cy = event.mouseMove.y;
                } break;
            default:
                break;
        }
        
    }
    
    
    window.clear();
    
    window.draw(background);
    
    ///window.draw(text);
    
    window.display();
}

