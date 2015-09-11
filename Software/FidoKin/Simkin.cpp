#include "Simkin.h"

Simkin::Simkin(int robWidth, int robHeight) {
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Fido Kinematic Simulator");
    
    sf::RectangleShape rect(sf::Vector2f(robWidth,robHeight));
    rect.setFillColor(sf::Color(0, 0, 0));
    
    int mLeft, mRight;
    
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

        int deg,trans;
        
        mToMove(rect,mLeft,mRight);
        
        window.draw(rect);
        window.display();
    } return EXIT_SUCCESS;
}

// creds to http://chess.eecs.berkeley.edu/eecs149/documentation/differentialDrive.pdf
void Simkin::mToMove(sf::RectangleShape& rect,int mLeft,int mRight) {
    double length = rect.getSize().x;
    double radius = (length/2)*(mLeft+mRight)/(mRight-mLeft);
    double omega = (mRight-mLeft)/length;
    
    double rX = rect.getPosition().x + length/2;
    double rY = rect.getPosition().y + rect.getSize().y/2;
    double rTheta = rect.getRotation()*0.0174532925;
    
    double delta = 5.32; // placeholder
    double time = 5.21; // this as well
    
    double iccX = rX - radius*sin(rTheta);
    double iccY = rY + radius*cos(rTheta);
    
    //matrices fun
    double xprime = cos(omega*delta*time)*(rX-iccX)+
                   -sin(omega*delta*time)*(rY-iccY)+
                    iccX;
    double yprime = sin(omega*delta*time)*(rX-iccX)+
                    cos(omega*delta*time)*(rY-iccY)+
                    iccY;
    double rprime = rTheta + omega*delta*time;
    
    rect.setPosition(xprime,yprime);
}