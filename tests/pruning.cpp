#include <iostream>
#include <algorithm>
#include <fstream>
#include "catch.h"
#include "../include/NeuralNet.h"
#include "../include/Pruner.h"
#include "../include/Backpropagation.h"

// Constants set for testing, could be anything
const static double ERROR_ALLOWANCE = 0.1;
const static int HIDDEN_LAYERS(1);
const static int NEURONS(10);
const static std::vector< std::vector<double> > input = { {1}, {2}, {3} };
const static std::vector< std::vector<double> > correctOutput = { {2}, {4}, {6} };

TEST_CASE("Test pruning", "[pruner]") {
  srand(time(NULL));
  net::Pruner pruner;
  double error1, error2;

  for(unsigned int a = 0; a < 1000; a++) {
    std::cout << a << ", " << error1 << ", " << error2 << "\n";
    net::NeuralNet neuralNetwork1(input[0].size(), correctOutput[0].size(), HIDDEN_LAYERS, NEURONS, "sigmoid");
    neuralNetwork1.setOutputActivationFunction("simpleLinear");
    net::NeuralNet neuralNetwork2(neuralNetwork1);

    net::Backpropagation initialTrainer = net::Backpropagation(0.1, 0.9, 0.01, 10000);
    initialTrainer.train(&neuralNetwork1, input, correctOutput);
    initialTrainer.train(&neuralNetwork2, input, correctOutput);

    pruner.prune(&neuralNetwork1, &initialTrainer);
    pruner.pruneRandomnly(&neuralNetwork2);

    net::Backpropagation quickTrainer = net::Backpropagation(0.1, 0.9, 0.00, 50);
    quickTrainer.train(&neuralNetwork1, input, correctOutput);
    quickTrainer.train(&neuralNetwork2, input, correctOutput);

    for(const auto &current: input) {
      error1 += fabs(current[0]*2 - neuralNetwork1.getOutput(current)[0]);
      error2 += fabs(current[0]*2 - neuralNetwork2.getOutput(current)[0]);
    }
  }

  REQUIRE(((error2 - error1) / error2) < 0.1);
}
