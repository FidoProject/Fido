#include "Simlink.h"


Simlink::Simlink() {
	windowThread = std::thread(&Simlink::updateLoop, this);
}

void Simlink::updateLoop() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	window.create(sf::VideoMode(1200, 900), "Fido Simulator", sf::Style::Default, settings);

	irVal = motors.motorOne = motors.motorTwo = 0;
	micVal = 30;
	visVal = batVal = 50;
	tempVal = 50;

	sf::Texture texture;
	if (!texture.loadFromFile("C:/Users/Michael/Documents/Fido/Software/FidoSim/background.png")) {
		exit(EXIT_FAILURE);
	} background = sf::Sprite(texture);

	sf::Font font;
	if (!font.loadFromFile("C:/Users/Michael/Documents/Fido/Software/FidoSim/sansation.ttf")) {
		exit(EXIT_FAILURE);
	} sf::Text text("Hello SFML", font, 50);
	text.setColor(sf::Color::Black);

	sf::Music music;
	if (!music.openFromFile("C:/Users/Michael/Documents/Fido/Software/FidoSim/nice_music.ogg")) {
		exit(EXIT_FAILURE);
	} music.play();

	click = false;

	while (true) updateWindow();
}

void Simlink::updateWindow() {
	if (window.isOpen() == false) return;

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
		if (cx > 160 && cx < 220 && cy > 20 && cy < 280)
			imu.accel.xComp = -((cy - 20) - 130) / 1.28;
		else if (cx > 270 && cx < 330 && cy > 20 && cy < 280)
			imu.accel.yComp = -((cy - 20) - 130) / 1.28;
		else if (cx > 380 && cx < 440 && cy > 20 && cy < 280)
			imu.accel.zComp = -((cy - 20) - 130) / 1.28;

		if (cx > 160 && cx < 220 && cy > 320 && cy < 580)
			imu.gyro.xComp = -((cy - 320) - 130) / 1.28;
		else if (cx > 270 && cx < 330 && cy > 320 && cy < 580)
			imu.gyro.yComp = -((cy - 320) - 130) / 1.28;
		else if (cx > 380 && cx < 440 && cy > 320 && cy < 580)
			imu.gyro.zComp = -((cy - 320) - 130) / 1.28;

		if (cx > 160 && cx < 220 && cy > 620 && cy < 880)
			imu.compass.xComp = -((cy - 620) - 130) / 1.28;
		else if (cx > 270 && cx < 330 && cy > 620 && cy < 880)
			imu.compass.yComp = -((cy - 620) - 130) / 1.28;
		else if (cx > 380 && cx < 440 && cy > 620 && cy < 880)
			imu.compass.zComp = -((cy - 620) - 130) / 1.28;

		if (cx > 780 && cx < 850 && cy > 620 && cy < 880)
			batVal = (880 - cy) / 2.6;
		else if (cx > 890 && cx < 960 && cy > 620 && cy < 880)
			irVal = (880 - cy) / 2.6;
		else if (cx > 1000 && cx < 1070 && cy > 620 && cy < 880)
			visVal = (880 - cy) / 2.6;
		else if (cx > 1110 && cx < 1180 && cy > 620 && cy < 880)
			micVal = (880 - cy) / 2.6;
		else if (cx > 500 && cx < 700 && cy > 800 && cy < 880)
			tempVal = (cx - 500) / 2;

	}

	sf::CircleShape ledCirc(40);
	ledCirc.setFillColor(sf::Color(led.r, led.g, led.b));
	ledCirc.setPosition(675, 625);
	ledCirc.setOutlineThickness(5);
	ledCirc.setOutlineColor(sf::Color(0, 0, 0));

	sf::RectangleShape slide(sf::Vector2f(40, 30));
	slide.setFillColor(sf::Color(0, 0, 0));

	window.clear();
	window.draw(background);
	window.draw(ledCirc);

	slide.setPosition(170, 133 - imu.accel.xComp*1.2);
	window.draw(slide);
	slide.setPosition(280, 133 - imu.accel.yComp*1.2);
	window.draw(slide);
	slide.setPosition(390, 133 - imu.accel.zComp*1.2);
	window.draw(slide);

	slide.setPosition(168, 435 - imu.gyro.xComp*1.2);
	window.draw(slide);
	slide.setPosition(278, 435 - imu.gyro.yComp*1.2);
	window.draw(slide);
	slide.setPosition(388, 435 - imu.gyro.zComp*1.2);
	window.draw(slide);

	slide.setPosition(166, 732 - imu.compass.xComp*1.2);
	window.draw(slide);
	slide.setPosition(276, 732 - imu.compass.yComp*1.2);
	window.draw(slide);
	slide.setPosition(386, 732 - imu.compass.zComp*1.2);
	window.draw(slide);

	slide.setPosition(797, 858 - batVal*2.5);
	window.draw(slide);
	slide.setPosition(906, 858 - irVal*2.5);
	window.draw(slide);
	slide.setPosition(1016, 858 - visVal*2.5);
	window.draw(slide);
	slide.setPosition(1126, 858 - micVal*2.5);
	window.draw(slide);

	sf::RectangleShape horizontalSlide(sf::Vector2f(30, 40));
	horizontalSlide.setFillColor(sf::Color(0, 0, 0));

	horizontalSlide.setPosition(500 + tempVal*2, 835);
	window.draw(horizontalSlide);

	sf::RectangleShape mOneLine(sf::Vector2f(20, abs(motors.motorOne) * 2));
	sf::RectangleShape mTwoLine(sf::Vector2f(20, abs(motors.motorTwo) * 2));
	mOneLine.setFillColor(sf::Color(0, 0, 0));
	mTwoLine.setFillColor(sf::Color(0, 0, 0));

	if (motors.motorOne > 0) {
		mOneLine.setPosition(680, 230 - motors.motorOne * 2);
		window.draw(mOneLine);
	}
	else if (motors.motorOne < 0) {
		mOneLine.setPosition(680, 370);
		window.draw(mOneLine);
	} if (motors.motorTwo > 0) {
		mTwoLine.setPosition(980, 230 - motors.motorTwo * 2);
		window.draw(mTwoLine);
	}
	else if (motors.motorTwo < 0) {
		mTwoLine.setPosition(980, 370);
		window.draw(mTwoLine);
	}

	std::string pText = "Vol: " + std::to_string(piezo.volume)
		+ "\nFreq: " + std::to_string(piezo.frequency);

	/**
	sf::Font font;
	font.loadFromFile("times.ttf");
	sf::Text piezoText("Hello guyz",font);
	piezoText.setCharacterSize(50);
	piezoText.setPosition(1000,500);
	window.draw(piezoText);
	**/

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

void Simlink::setLED(int r, int g, int b, int i) {
    led.r = r * i/100;
    led.g = g * i/100;
    led.b = b * i/100;
}

void Simlink::setMotors(int motorOne, int motorTwo) {
    motors.motorOne = motorOne;
    motors.motorTwo = motorTwo;
}

void Simlink::chirp(int volume, int frequency) {
    piezo.frequency = frequency;
    piezo.volume = volume;
    int endTime = 200 + clock();
    while(clock() != endTime);
    piezo.volume = 0;
}

int Simlink::getTemperature() {
	return tempVal;
}