#include <cstdio>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "catch.h"
#include "../src/FidoSim/Simlink.h"

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
}
