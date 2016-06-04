#include <algorithm>
#include <fstream>
#include <iostream>
#include "catch.h"
#include "../include/NeuralNet.h"
#include "../include/Backpropagation.h"

const static double ERROR_ALLOWANCE = 0.1;
const static double DIFFERENCE_ALLOWANCE = 0.001;
const static char * FILENAME = "temp/backpropfile.txt";

TEST_CASE("Test back propagation", "[backprop]") {
  // Creates a neural network with
  // 1 input, 1 output, 2 hidden layers, 4 neurons per hidden layer,
  // a sigmoid activation function for hidden layers, and a linear activation function on the final layer.
  net::NeuralNet neuralNetwork = net::NeuralNet(1, 1, 2, 4, "sigmoid");
  neuralNetwork.setOutputActivationFunction("simpleLinear");

  std::vector< std::vector<double> > input = { {1}, {2}, {5}, {6} };
  std::vector< std::vector<double> > correctOutput = { {2}, {4}, {10}, {12} };

  net::Backpropagation backprop = net::Backpropagation(0.1, 0.2, 0.001, 10000);
  backprop.train(&neuralNetwork, input, correctOutput);

  // Cycle through inputs and print the outputs
  for(std::vector<double> current: input) {
    REQUIRE(fabs(current[0]*2 - neuralNetwork.getOutput(current)[0]) < ERROR_ALLOWANCE);
  }
}

TEST_CASE("Save and load backpropagations through streams", "[backprop]") {
  // Creates a neural network with
  // 1 input, 1 output, 2 hidden layers, 4 neurons per hidden layer,
  // a sigmoid activation function for hidden layers, and a linear activation function on the final layer.
  net::NeuralNet neuralNetwork = net::NeuralNet(1, 1, 2, 4, "sigmoid");
  neuralNetwork.setOutputActivationFunction("simpleLinear");
  net::NeuralNet newNeuralNetwork = net::NeuralNet(1, 1, 2, 4, "sigmoid");
  newNeuralNetwork.setOutputActivationFunction("simpleLinear");
  newNeuralNetwork.net = neuralNetwork.net;

  std::vector< std::vector<double> > input = { {1}, {2}, {5}, {6} };
  std::vector< std::vector<double> > correctOutput = { {2}, {4}, {10}, {12} };

  // Create backpropagation object with
  // a learning rate of 10%, a momentum term of 0.001, an acceptable error level of 0.1%,
  // and a maximum number of training iterations of 10000
  net::Backpropagation backprop = net::Backpropagation(0.1, 0.001, 0.001, 10000);

  // Save
  std::ofstream ostream;
  ostream.open(FILENAME, std::ofstream::out | std::ofstream::trunc);
  backprop.store(&ostream); // Store old
  ostream.close();

  // Load
  std::ifstream istream;
  istream.open(FILENAME, std::ifstream::in);
  net::Backpropagation newBackprop = net::Backpropagation(&istream); // load new
  istream.close();

  REQUIRE(backprop.learningRate == newBackprop.learningRate);
  REQUIRE(backprop.momentumTerm == newBackprop.momentumTerm);
  REQUIRE(backprop.targetErrorLevel == newBackprop.targetErrorLevel);
  REQUIRE(backprop.maximumEpochs == newBackprop.maximumEpochs);

  // Train with new backprop
  backprop.train(&neuralNetwork, input, correctOutput); // train old
  newBackprop.train(&newNeuralNetwork, input, correctOutput); // train new

  // Test similarity
  for(std::vector<double> current: input) {
    REQUIRE( fabs(newNeuralNetwork.getOutput(current)[0] - neuralNetwork.getOutput(current)[0]) < DIFFERENCE_ALLOWANCE);
  }
}
