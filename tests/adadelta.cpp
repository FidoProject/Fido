#include <algorithm>
#include <fstream>
#include <iostream>
#include "catch.h"
#include "../include/NeuralNet.h"
#include "../include/Adadelta.h"

const static double ERROR_ALLOWANCE = 0.1;
const static double DIFFERENCE_ALLOWANCE = 0.001;
const static char * FILENAME = "temp/backpropfile.txt";

TEST_CASE("Test adadelta", "[adadelta]") {
  net::NeuralNet neuralNetwork = net::NeuralNet(1, 1, 2, 4, "sigmoid");
  neuralNetwork.setOutputActivationFunction("simpleLinear");

  std::vector< std::vector<double> > input = { {1}, {2}, {5}, {6} };
  std::vector< std::vector<double> > correctOutput = { {2}, {4}, {10}, {12} };

  net::Adadelta trainer = net::Adadelta(0.95, 0.0005, 10000);
  trainer.train(&neuralNetwork, input, correctOutput);

  for(std::vector<double> current: input) {
    REQUIRE(fabs(current[0]*2 - neuralNetwork.getOutput(current)[0]) < ERROR_ALLOWANCE);
  }
}
