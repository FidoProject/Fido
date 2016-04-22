#include <iostream>
#include <algorithm>
#include <fstream>
#include "catch.h"
#include "../include/NeuralNet.h"
#include "../include/Pruner.h"

// Constants set for testing, could be anything
const static double ERROR_ALLOWANCE = 0.1;
const static int HIDDEN_LAYERS(1);
const static int NEURONS(20);
const static int MAX_FINAL_NEURONS(15);
const static std::vector< std::vector<double> > input = { {1}, {2}, {3} };
const static std::vector< std::vector<double> > correctOutput = { {2}, {4}, {6} };

TEST_CASE("Test pruning", "[pruner]") {
	net::NeuralNet neuralNetwork = net::NeuralNet(input[0].size(), correctOutput[0].size(), HIDDEN_LAYERS, int(NEURONS/double(HIDDEN_LAYERS+1)), "sigmoid");
	neuralNetwork.setOutputActivationFunction("simpleLinear");

	net::Pruner pruner = net::Pruner(0.1, 0, -1, 1000, 100);
	pruner.train(&neuralNetwork, input, correctOutput);

	for (std::vector<double> current: input) {
		REQUIRE(fabs(current[0]*2 - neuralNetwork.getOutput(current)[0]) < ERROR_ALLOWANCE);
	}

	REQUIRE(neuralNetwork.numberOfHiddenNeurons() < MAX_FINAL_NEURONS);


}
