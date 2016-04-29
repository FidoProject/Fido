#include <iostream>
#include "catch.h"
#include "../include/QLearn.h"
#include "../include/NeuralNet.h"
#include "../include/Adadelta.h"

TEST_CASE("Learn to mimic ", "[qlearn]") {
	srand(time(NULL));

  rl::QLearn learner(new net::NeuralNet(1, 1, 1, 2, "sigmoid"), new net::Adadelta(0.9, 0.05, 1000), 1, 0, {{0}, {1}});
  for(int a = 0; a < 100; a++) {
    int selector = rand() % 2;
    int action = (int)learner.chooseBestAction({selector})[0];
    learner.applyReinforcementToLastAction(selector == action ? 1 : -1, {selector});
  }

  REQUIRE((int)learner.chooseBestAction({0})[0]  == 0);
  REQUIRE((int)learner.chooseBestAction({1})[0]  == 1);
}
