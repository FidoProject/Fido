#include "Simkin.h"

Simkin::Simkin(int robWidth, int robHeight) {
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Fido Kinematic Simulator");
    
    sf::RectangleShape rect(sf::Vector2f(robWidth,robHeight));
    rect.setFillColor(sf::Color(0, 0, 0));
    rect.setPosition(500,400);
    rect.setOrigin(robWidth/2,robHeight/2);
    rect.setRotation(90);
    
    int mLeft, mRight;
    mLeft = mRight = 0;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                default:
                    break;
            }
        } window.clear(sf::Color(255,255,255));
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) mLeft=100;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) mLeft=-100;
        else mLeft = 0;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) mRight=100;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mRight=-100;
        else mRight = 0;
        
        mToMove(rect,mLeft,mRight,1);
        
        window.draw(rect);
        window.display();
        sf::sleep(sf::milliseconds(25));
    } return EXIT_SUCCESS;
}

// creds to http://chess.eecs.berkeley.edu/eecs149/documentation/differentialDrive.pdf
void Simkin::mToMove(sf::RectangleShape& rect,int motLeft,int motRight,double speed) {
    double mLeft = -(((double)motLeft)/200)*speed;
    double mRight = -(((double)motRight)/200)*speed;
    
    double xprime,yprime,rprime;
    double length = rect.getSize().x;
    double time = moveClock.restart().asMilliseconds();
    
    double rX = rect.getPosition().x;
    double rY = rect.getPosition().y;
    double rTheta = rect.getRotation()*0.0174532925;

    if (mLeft == mRight) {
        xprime = rX + mLeft*cos(rTheta)*time;
        yprime = rY + mLeft*sin(rTheta)*time;
        rprime = rTheta;
    } else {
        double radius = (length/2)*(mLeft+mRight)/(mRight-mLeft);
        double omega = (mRight-mLeft)/length;

        double iccX = rX - radius*sin(rTheta);
        double iccY = rY + radius*cos(rTheta);

        xprime = iccX + (cos(omega*time)*(rX-iccX)+
                -sin(omega*time)*(rY-iccY));
        yprime = iccY + (sin(omega*time)*(rX-iccX)+
                 cos(omega*time)*(rY-iccY));
        rprime = rTheta + omega*time;
    }
    
    rect.setPosition(xprime,yprime);
    rect.setRotation(rprime*57.2957795);
}