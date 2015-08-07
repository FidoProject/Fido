#include "NeuralNet.h"

using namespace std;

NeuralNet::NeuralNet(int numInputs, int numOutputs, int numHiddenLayers, int numNeruronsPerHiddenLayer) {
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

double NeuralNet::sigmoid(double activiation) {
	return ( 1 / ( 1 + exp(-activiation / 1)));
}