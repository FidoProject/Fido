#include "NeuralNet.h"

using namespace std;

// Initializes a neural network
NeuralNet::NeuralNet(int numInputs_, int numOutputs_, int numHiddenLayers_, int numNeruronsPerHiddenLayer_) {
	numInputs = numInputs_;
	numOutputs = numOutputs_;
	numHiddenLayers = numHiddenLayers_;
	numNeruronsPerHiddenLayer = numNeruronsPerHiddenLayer_;

	vector<Neuron> firstHiddenLayer;
	for(int a = 0; a < numHiddenLayers; a++) firstHiddenLayer.push_back(Neuron(numInputs));
	net.push_back(firstHiddenLayer);

	for(int a = 0; a < numHiddenLayers; a++) {
		vector<Neuron> hiddenLayer;
		for(int b = 0; b < numNeruronsPerHiddenLayer; b++) hiddenLayer.push_back(Neuron(numHiddenLayers));
		net.push_back(hiddenLayer);
	}

	vector<Neuron> outputLayer;
	for(int a = 0; a < numOutputs; a++) outputLayer.push_back(Neuron(numNeruronsPerHiddenLayer));
	net.push_back(outputLayer);
}

// Gets the weights of each neuron in the net and organizizes them into a vector starting from the first neuron in the first layer
// to the last neuron in the output layer
vector<double> NeuralNet::getWeights() {
	vector<double> weights;
	for(int a = 0; a < net.size(); a++) {
		for(int b = 0; b < net[a].size(); b++) {
			for(int c = 0; c < net[a][b].weights.size(); c++) {
				weights.push_back(net[a][b].weights[c]);
			}
		}
	}
	return weights;
}


// Sets the weights of each neuron in the net according to a vector.
// Starts at the first neuron in the first layer and goes to the last neruon in the last layer
void NeuralNet::setWeights(vector<double> w) {
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

// The crowning function of this class.
// Neural network produces an output based on an input vector of doubles
vector<double> NeuralNet::getOutput(vector<double> input) {
	vector<double> output;
	for(int a = 0; a < net.size(); a++) {
		if(a > 0) input = output;
		output.clear();
		for(int b = 0; b < net[a].size(); b++) {
			output.push_back(sigmoid(net[a][b].getOutput(input)));
		}
	}
	return output;
}


// A s-shaped math function
// Each neuron takes the summation of their inputs times their respective weights and plugs it into this fuction to get a gradient output  
double NeuralNet::sigmoid(double activiation) {
	return ( 1 / ( 1 + exp(-activiation / 1)));
}