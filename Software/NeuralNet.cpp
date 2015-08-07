#include "NeuralNet.h"

using namespace net;

NeuralNet::NeuralNet(int numInputs_, int numOutputs_, int numHiddenLayers_, int numNeuronsPerHiddenLayer_) {
	numInputs = numInputs_;
	numOutputs = numOutputs_;
	numHiddenLayers = numHiddenLayers_;
	numNeuronsPerHiddenLayer = numNeuronsPerHiddenLayer_;

	std::vector<Neuron> firstHiddenLayer;
	for(int a = 0; a < numHiddenLayers; a++) firstHiddenLayer.push_back(Neuron(numInputs));
	net.push_back(firstHiddenLayer);

	for(int a = 0; a < numHiddenLayers; a++) {
		std::vector<Neuron> hiddenLayer;
		for(int b = 0; b < numNeuronsPerHiddenLayer; b++) hiddenLayer.push_back(Neuron(numHiddenLayers));
		net.push_back(hiddenLayer);
	}

	std::vector<Neuron> outputLayer;
	for(int a = 0; a < numOutputs; a++) outputLayer.push_back(Neuron(numNeuronsPerHiddenLayer));
	net.push_back(outputLayer);
}

NeuralNet::NeuralNet(NeuralNet otherNet) {
	numInputs = otherNet.numInputs;
	numOutputs = otherNet.numOutputs;
	numHiddenLayers = otherNet.numHiddenLayers;
	numNeuronsPerHiddenLayer = otherNet.numNeuronsPerHiddenLayer;

	std::vector<Neuron> firstHiddenLayer;
	for(int a = 0; a < numHiddenLayers; a++) firstHiddenLayer.push_back(Neuron(numInputs));
	net.push_back(firstHiddenLayer);

	for(int a = 0; a < numHiddenLayers; a++) {
		std::vector<Neuron> hiddenLayer;
		for(int b = 0; b < numNeuronsPerHiddenLayer; b++) hiddenLayer.push_back(Neuron(numHiddenLayers));
		net.push_back(hiddenLayer);
	}

	std::vector<Neuron> outputLayer;
	for(int a = 0; a < numOutputs; a++) outputLayer.push_back(Neuron(numNeuronsPerHiddenLayer));
	net.push_back(outputLayer);
}

std::vector<double> NeuralNet::getWeights() {
	std::vector<double> weights;
	for(int a = 0; a < net.size(); a++) {
		for(int b = 0; b < net[a].size(); b++) {
			for(int c = 0; c < net[a][b].weights.size(); c++) {
				weights.push_back(net[a][b].weights[c]);
			}
		}
	}
	return weights;
}

void NeuralNet::setWeights(std::vector<double> w) {
	int counter = 0;
	for(int a = 0; a < net.size(); a++) {
		for(int b = 0; b < net[a].size(); b++) {
			for(int c = 0; c < net[a][b].weights.size(); c++) {
				net[a][b].weights[c] = w[counter];
				counter++;
			}
		}
	}
}

std::vector<double> NeuralNet::getOutput(std::vector<double> input) {
	std::vector<double> output;
	for(int a = 0; a < net.size(); a++) {
		if(a > 0) input = output;
		output.clear();
		for(int b = 0; b < net[a].size(); b++) {
			output.push_back(sigmoid(net[a][b].getOutput(input)));
		}
	}
	return output;
}

double NeuralNet::sigmoid(double activiation) {
	return ( 1 / ( 1 + exp(-activiation / 1)));
}