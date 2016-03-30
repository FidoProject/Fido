#include <cstdio>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "catch.h"
#include "../include/Simulator/Simlink.h"

TEST_CASE( "Sensors", "[simulator]" ) {
	Simlink simlink;
	SECTION("Line following") {
		simlink.robot.setPosition(500, 200);
		simlink.placeLine(sf::Vector2f(0, 0), sf::Vector2f(100, 100));
		REQUIRE(simlink.isLeftOfLine() == 0);
		REQUIRE(fabs(simlink.distanceFromLine() - 212.13203) < 0.001);
		simlink.robot.setPosition(0, 500);
		REQUIRE(fabs(simlink.distanceFromLine() - 353.55339) < 0.001);
	}

	SECTION("Z-X Displacement sensor") {
		double x, z;

		simlink.robot.setPosition(500, 200);
		simlink.ball.setPosition(400, 100);
		simlink.getBallDisplacement(&x, &z);
		REQUIRE(x == 100);
		REQUIRE(z == 100);

		simlink.robot.setRotation(45);
		simlink.robot.setPosition(500, 200);
		simlink.ball.setPosition(853.548, 129.288);
		simlink.getBallDisplacement(&x, &z);
		REQUIRE(fabs(x+300) < 0.01);
		REQUIRE(fabs(z+200) < 0.01);
	}

	SECTION("Kiwi drive") {
		double x = 500, y = 200, r = 0, dx = 100, dy = 100, dr = 0;

		simlink.robot.setPosition(x, y);
		simlink.robot.setRotation(r);
		simlink.robot.inverseGoKiwi(dx, dy, dr, 1);
		std::cout << "Simlink " << simlink.robot.getPosition().x << "\n";
		REQUIRE(simlink.robot.getPosition().x == x+dx);
		REQUIRE(simlink.robot.getPosition().y ==  y+dy);
		REQUIRE(simlink.robot.getRotation() == r+dr);
	}
}
