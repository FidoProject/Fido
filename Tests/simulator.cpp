#include <cstdio>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "catch.h"
#include "../Software/FidoSim/Simlink.h"

TEST_CASE( "Line following sensor", "[]" ) {
	Simlink simlink;
	SECTION("Side and distance from line") {
		simlink.robot.setPosition(500, 200);
		REQUIRE(simlink.isLeftOfLine() == 0);
		REQUIRE(fabs(simlink.distanceFromLine() - 212.13203) < 0.001);
		simlink.robot.setPosition(0, 500);
		REQUIRE(fabs(simlink.distanceFromLine() - 353.55339) < 0.001);
	}
}