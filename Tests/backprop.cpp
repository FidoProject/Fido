#include <algorithm>
#include "catch.h"
#include "../Software/NeuralNet.h"
#include "../Software/Backpropagation.h"

const double ERROR_ALLOWANCE = 1.0;

TEST_CASE("Test back propagation", "[backprop]") {
	// Creates a neural network with
	// 1 input, 1 output, 2 hidden layers, 4 neurons per hidden layer,
	// a sigmoid activation function for hidden layers, and a linear activation function on the final layer.
	net::NeuralNet neuralNetwork = net::NeuralNet(1, 1, 2, 4, "sigmoid");
	neuralNetwork.setOutputActivationFunction("simpleLinear");

	std::vector< std::vector<double> > input = { {1}, {2}, {5}, {6} };
	std::vector< std::vector<double> > correctOutput = { {2}, {4}, {10}, {12} };

	// Create backpropagation object with
	// a learning rate of 10%, a momentum term of 0.001, an acceptable error level of 0.1%,
	// and a maximum number of training iterations of 10000
	net::Backpropagation backprop = net::Backpropagation(0.1, 0.001, 0.001, 10000);
	backprop.trainOnData(&neuralNetwork, input, correctOutput);

	// Cycle through inputs and print the outputs
	for (std::vector<double> current: input) {
		REQUIRE(fabs(current[0]*2 - neuralNetwork.getOutput(current)[0]) < ERROR_ALLOWANCE);
	}
}
