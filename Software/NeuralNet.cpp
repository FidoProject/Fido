#include "NeuralNet.h"

#include <iostream>

#include <sstream>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>

using namespace net;

NeuralNet::NeuralNet() {
}

NeuralNet::NeuralNet(int numInputs, int numOutputs, int numHiddenLayers, int numNeuronsPerHiddenLayer, std::string activationFunctionName) {
	Layer firstHiddenLayer;
	for (int a = 0; a < numNeuronsPerHiddenLayer; a++) firstHiddenLayer.neurons.push_back(Neuron(numInputs));
	net.push_back(firstHiddenLayer);

	for (int a = 0; a < numHiddenLayers - 1; a++) {
		Layer hiddenLayer;
		for (int b = 0; b < numNeuronsPerHiddenLayer; b++) hiddenLayer.neurons.push_back(Neuron(numNeuronsPerHiddenLayer));
		net.push_back(hiddenLayer);
	}

	Layer outputLayer;
	for (int a = 0; a < numOutputs; a++) outputLayer.neurons.push_back(Neuron(numNeuronsPerHiddenLayer));
	net.push_back(outputLayer);

	for (int a = 0; a < net.size(); a++) net[a].setActivationFunctionWithName(activationFunctionName);
}

NeuralNet::NeuralNet(NeuralNet* otherNet) {
	net = otherNet->net;
	randomizeWeights();
}

NeuralNet::NeuralNet(std::string filename) { 
	std::ifstream input;
	input.open(filename.c_str(), std::ifstream::in);
	initWithStream(&input);
	input.close();
}

NeuralNet::NeuralNet(std::ifstream *input) {
	initWithStream(input);
}

void NeuralNet::storeNet(std::string filename) {
	std::ofstream output;
	output.open(filename.c_str(), std::ios::app);
	storeNetWithStream(&output);
	output.close();
}

void NeuralNet::storeNetWithStream(std::ofstream *output) {
	if(output->is_open()) {
		// Output the net to the file
		*output << net.size() << "\n";
		for(int a = 0; a < net.size(); a++) {
			net[a].storeNetWithStream(output);
		}
	} else {
		std::cout << "Could not store neural network\n";
		throw 1;
	}
}

std::vector<double> NeuralNet::getWeights() {
	std::vector<double> weights;
	for(int a = 0; a < net.size(); a++) {
		for(int b = 0; b < net[a].neurons.size(); b++) {
			for(int c = 0; c < net[a].neurons[b].weights.size(); c++) {
				weights.push_back(net[a].neurons[b].weights[c]);
			}
		}
	}
	return weights;
}

std::vector< std::vector< std::vector<double> > > NeuralNet::getWeights3D() {
	std::vector< std::vector< std::vector<double> > > weights;
	for(int a = 0; a < net.size(); a++) {
		std::vector< std::vector<double> > layer;
		for(int b = 0; b < net[a].neurons.size(); b++) {
			layer.push_back(net[a].neurons[b].weights);
		}
		weights.push_back(layer);
	}
	return weights;
}

void NeuralNet::setWeights(std::vector<double> w) {
	int counter = 0;
	for(int a = 0; a < net.size(); a++) {
		for(int b = 0; b < net[a].neurons.size(); b++) {
			for(int c = 0; c < net[a].neurons[b].weights.size(); c++) {
				net[a].neurons[b].weights[c] = w[counter];
				counter++;
			}
		}
	}
	if(counter != w.size()) {
		std::cout << "Not the right number of weights\n";
		throw 1;
	}
}

void NeuralNet::setWeights3D(std::vector< std::vector< std::vector<double> > > w) {
	for(int a = 0; a < net.size(); a++) {
		for(int b = 0; b < net[a].neurons.size(); b++) {
			net[a].neurons[b].weights = w[a][b];
		}
	}
}

void NeuralNet::randomizeWeights() {
	for(int a = 0; a < net.size(); a++)  for(int b = 0; b < net[a].neurons.size(); b++) net[a].neurons[b].randomizeWeights();
}

std::vector<double> NeuralNet::getOutput(std::vector<double> input) {
	std::vector<double> output;
	for(int a = 0; a < net.size(); a++) {
		if(a > 0) input = output;
		output.clear();
		for(int b = 0; b < net[a].neurons.size(); b++) {
			double out = net[a].activationFunction(net[a].neurons[b].getOutput(input));
			output.push_back(out);
		}
	}
	return output;
}

std::vector< std::vector<double> > NeuralNet::feedForward(std::vector<double> input) {
	std::vector< std::vector<double> > output;
	for(int a = 0; a < net.size(); a++) {
		if(a > 0) input = output[output.size() - 1];

		std::vector<double> outputLayer;

		for(int b = 0; b < net[a].neurons.size(); b++) {
			double out = net[a].activationFunction(net[a].neurons[b].getOutput(input));
			outputLayer.push_back(out);
		}

		output.push_back(outputLayer);
	}
	return output;
}

void NeuralNet::initWithStream(std::ifstream *input) {
	if(input->is_open()) {
		int numLayers;
		*input >> numLayers;
		for (int a = 0; a < numLayers; a++) {
			net.push_back(Layer(input));
		}
	} else {
		std::cout << "Could not retrieve neural network from file\n";
		throw 1;
	}
}

void NeuralNet::printWeights() {
	std::cout << "Neuron weights: \n";
	for(int a = 0; a < net.size(); a++) {
		std::cout << "  Layer " << a << ":\n";
		for(int b = 0; b < net[a].neurons.size(); b++) {
			std::cout << "	  Neuron " << b << ": ";
			for(int c = 0; c < net[a].neurons[b].weights.size(); c++) {
				std::cout << net[a].neurons[b].weights[c] << " ";
			}
			std::cout << "\n";
		}
	}
}

int NeuralNet::numberOfHiddenLayers() {
	return net.size() - 1;
}

int NeuralNet::numberOfInputs() {
	return net[0].neurons[0].weights.size() - 1;
}

int NeuralNet::numberOfOutputs() {
	return net[net.size() - 1].neurons.size();
}

void NeuralNet::setOutputActivationFunction(std::string name) {
	net[net.size() - 1].setActivationFunctionWithName(name);
}
