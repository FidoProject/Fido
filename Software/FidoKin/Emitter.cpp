//
//  Emitter.cpp
//  FidoKin
//
//  Created by Joshua Gruenstein on 9/11/15.
//  Copyright (c) 2015 fido. All rights reserved.
//

#include "Emitter.h"

Emitter::Emitter(int str):sf::CircleShape(0) {
    setFillColor(sf::Color(255,0,0));
    setOutlineThickness(-3);
    setOutlineColor(sf::Color(0,0,0));
    setPointCount(100);
    masterStrength = str;
}

void Emitter::set(sf::Vector2i mouseLoc) {
    setRadius(15);
    strength = masterStrength;
    setPosition(static_cast<sf::Vector2f>(mouseLoc).x - getRadius(),
                static_cast<sf::Vector2f>(mouseLoc).y - getRadius());
}

void Emitter::bye() {
    setRadius(0);
    strength = 0;
}