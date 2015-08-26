#include "Simlink.h"

Simlink::Simlink() {
    window.create(sf::VideoMode(1200, 900), "Fido Simulator");
    
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
    
    click = false;
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
    
    if (click) {
        if (cx>160 && cx<220 && cy > 20 && cy < 280)
            imu.accel.xComp = -((cy - 20) - 130)/1.28;
        else if (cx>270 && cx<330 && cy > 20 && cy < 280)
            imu.accel.yComp = -((cy - 20) - 130)/1.28;
        else if (cx>380 && cx<440 && cy > 20 && cy < 280)
            imu.accel.zComp = -((cy - 20) - 130)/1.28;
        
        if (cx>160 && cx<220 && cy > 320 && cy < 580)
            imu.gyro.xComp = -((cy - 320) - 130)/1.28;
        else if (cx>270 && cx<330 && cy > 320 && cy < 580)
            imu.gyro.yComp = -((cy - 320) - 130)/1.28;
        else if (cx>380 && cx<440 && cy > 320 && cy < 580)
            imu.gyro.zComp = -((cy - 320) - 130)/1.28;
        
        if (cx>160 && cx<220 && cy > 620 && cy < 880)
            imu.compass.xComp = -((cy - 620) - 130)/1.28;
        else if (cx>270 && cx<330 && cy > 620 && cy < 880)
            imu.compass.yComp = -((cy - 620) - 130)/1.28;
        else if (cx>380 && cx<440 && cy > 620 && cy < 880)
            imu.compass.zComp = -((cy - 620) - 130)/1.28;
        
        if (cx>780 && cx<850 && cy > 620 && cy < 880)
            batVal = (880-cy)/2.6;
        else if (cx>890 && cx<960 && cy > 620 && cy < 880)
            irVal = (880-cy)/2.6;
        else if (cx>1000 && cx<1070 && cy > 620 && cy < 880)
            visVal = (880-cy)/2.6;
        else if (cx>1110 && cx<1180 && cy > 620 && cy < 880)
            micVal = (880-cy)/2.6;
    }
    
    window.clear();
    window.draw(background);
    window.display();
}

Simlink::TDVect Simlink::getAccel() {
    return imu.accel;
}

Simlink::TDVect Simlink::getGyro() {
    return imu.gyro;
}

Simlink::TDVect Simlink::getCompass() {
    return imu.compass;
}

int Simlink::getMicrophone() {
    return micVal;
}

int Simlink::getIR() {
    return irVal;
}

int Simlink::getVis() {
    return visVal;
}

int Simlink::getBattery() {
    return batVal;
}