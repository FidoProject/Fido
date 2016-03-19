#include "Software/NeuralNet.h"
#include "Software/Backpropagation.h"
#include <iostream>

/*
	This is example code which runs a Backpropagating neural network.
	It is supposed to double any input number.
	This is the example code from the readme of https://github.com/FidoProject/Fido

	To compile, run in terminal:
	fcc linear.cpp
	This will make an executable file a.out which will be this program.

	PS.
	Try tweaking the paramaters of the backprop initialization to make the nerual network more correct
*/

int main() {
	// Creates a neural network with
	// 1 input, 1 output, 2 hidden layers, 4 neurons per hidden layer, and a sigmoid activation function.
	net::NeuralNet neuralNetwork = net::NeuralNet(1, 1, 2, 4, "sigmoid");
	std::vector< std::vector<double> > input = { {1}, {2}, {5}, {6} };
	std::vector< std::vector<double> > correctOutput = { {2}, {4}, {10}, {12} };
	neuralNetwork.setOutputActivationFunction("simpleLinear");

	// Create backpropagation object with
	// a learning rate of 10%, a momentum term of 0.001, an acceptable error level of 5%,
	// and a maximum number of training iterations of 10000
	net::Backpropagation backprop = net::Backpropagation(0.1, 0.001, 0.05, 1000000);
	backprop.trainOnData(&neuralNetwork, input, correctOutput);

	// Cycle through inputs and print the outputs
	for (std::vector<double> current: input) {
		std::vector<double> final = neuralNetwork.getOutput(current);
		std::cout << "Correct answer: " << current[0] << "\tActual answer:" << final[0] << std::endl;
	}
}
