#include "NeuralNet.h"

#include <iostream>

#include <sstream>
#include <vector>
#include <math.h>
#include <string>

using namespace net;

NeuralNet::NeuralNet() {
	numInputs = 0;
	numOutputs = 0;
	numHiddenLayers = 0;
	numNeuronsPerHiddenLayer = 0;
}

NeuralNet::NeuralNet(int numInputs_, int numOutputs_, int numHiddenLayers_, int numNeuronsPerHiddenLayer_, std::string activationFunctionName) {
	numInputs = numInputs_;
	numOutputs = numOutputs_;
	numHiddenLayers = numHiddenLayers_;
	numNeuronsPerHiddenLayer = numNeuronsPerHiddenLayer_;
	setHiddenActivationFunction(activationFunctionName);
	setOutputActivationFunction(activationFunctionName);

    setupNeuronLayers();
}

NeuralNet::NeuralNet(NeuralNet* otherNet) {
	numInputs = otherNet->numInputs;
	numOutputs = otherNet->numOutputs;
	numHiddenLayers = otherNet->numHiddenLayers;
	numNeuronsPerHiddenLayer = otherNet->numNeuronsPerHiddenLayer;
	outputActivationFunction = otherNet->outputActivationFunction;
	hiddenActivationFunction = otherNet->hiddenActivationFunction;

	setupNeuronLayers();
}

NeuralNet::NeuralNet(std::string filename) { 
	std::ifstream input(filename);
	initWithStream(&input);
	input.close();
}

NeuralNet::NeuralNet(std::ifstream *input) {
	initWithStream(input);
}

void NeuralNet::storeNet(std::string filename) {
	std::ofstream output(filename);
	storeNetWithStream(&output);
	output.close();
}

void NeuralNet::storeNetWithStream(std::ofstream *output) {
	if(output->is_open()) {
		*output << numInputs << " " << numOutputs << " " << numHiddenLayers << " " << numNeuronsPerHiddenLayer << "\n";
		*output << getHiddenActivationFunctionName() << " " << getOutputActivationFunctionName() << "\n";

		std::vector<double> weights = getWeights();
		for(int a = 0; a < weights.size(); a++) *output << weights[a] << " ";
		std::cout << "\n";
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

std::vector< std::vector<double> > NeuralNet::getHiddenLayerWeights2D(int hiddenLayerIndex) {
	if(hiddenLayerIndex < 0 || hiddenLayerIndex > numHiddenLayers-1) {
		std::cout << "Invalid hidden layer index\n";
		throw 1;
	}
	std::vector< std::vector<double> > weights;
	for(int b = 0; b < net[hiddenLayerIndex].neurons.size(); b++) {
		weights.push_back(net[hiddenLayerIndex].neurons[b].weights);
	}
	return weights;
}

std::vector< std::vector<double> > NeuralNet::getOutputLayerWeights2D() {
	std::vector< std::vector<double> > weights;
	for(int b = 0; b < net[net.size()-1].neurons.size(); b++) {
		weights.push_back(net[net.size() - 1].neurons[b].weights);
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

void NeuralNet::setHiddenLayerWeights2D(int hiddenLayerIndex, std::vector< std::vector<double> > w) {
	for(int b = 0; b < net[hiddenLayerIndex].neurons.size(); b++) {
		for(int c = 0; c < net[hiddenLayerIndex].neurons[b].weights.size(); c++) {
			net[hiddenLayerIndex].neurons[b].weights[c] = w[b][c];
		}
	}
}

void NeuralNet::setOutputLayerWeights2D(std::vector< std::vector<double> > w) {
	for(int b = 0; b < net[net.size() - 1].neurons.size(); b++) {
		for(int c = 0; c < net[net.size() - 1].neurons[b].weights.size(); c++) {
			net[net.size() - 1].neurons[b].weights[c] = w[b][c];
		}
	}
}


std::vector<double> NeuralNet::getOutput(std::vector<double> input) {
	std::vector<double> output;
	for(int a = 0; a < net.size(); a++) {
		if(a > 0) input = output;
		output.clear();
		for(int b = 0; b < net[a].neurons.size(); b++) {
			double out;
			if(a == net.size() - 1) out = outputActivationFunction(net[a].neurons[b].getOutput(input));
			else out = hiddenActivationFunction(net[a].neurons[b].getOutput(input));
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
			double out;
			if(a == net.size() - 1) out = outputActivationFunction(net[a].neurons[b].getOutput(input));
			else out = hiddenActivationFunction(net[a].neurons[b].getOutput(input));
			outputLayer.push_back(out);
		}

		output.push_back(outputLayer);
	}
	return output;
}

std::map<std::string, ActivationFunction> NeuralNet::getActivationFunctionNameMap() {
	std::map<std::string, ActivationFunction> map;
	map["sigmoid"] = sigmoid;
	map["binary"] = binary;
	map["integer"] = integer;
	map["simpleLinear"] = simpleLinear;
    map["tanSigmoid"] = tanSigmoid;

	return map;
}

void NeuralNet::setHiddenActivationFunction(std::string name) {
	hiddenActivationFunction = getActivationFunctionNameMap()[name];
}

void NeuralNet::setOutputActivationFunction(std::string name) {
	outputActivationFunction = getActivationFunctionNameMap()[name];
}

std::string NeuralNet::getHiddenActivationFunctionName() {
	std::map<std::string, ActivationFunction> nameMap = getActivationFunctionNameMap();
	for(auto a = nameMap.begin(); a != nameMap.end(); ++a) if(a->second == hiddenActivationFunction) return a->first;
    
    throw 1;
}

std::string NeuralNet::getOutputActivationFunctionName() {
	std::map<std::string, ActivationFunction> nameMap = getActivationFunctionNameMap();
	for(auto a = nameMap.begin(); a != nameMap.end(); ++a) if(a->second == outputActivationFunction) return a->first;
    
    throw 1;
}

void NeuralNet::setupNeuronLayers() {
	Layer firstHiddenLayer;
    for(int a = 0; a < numNeuronsPerHiddenLayer; a++) firstHiddenLayer.neurons.push_back(Neuron(numInputs));
    net.push_back(firstHiddenLayer);
    
    for(int a = 0; a < numHiddenLayers-1; a++) {
        Layer hiddenLayer;
        for(int b = 0; b < numNeuronsPerHiddenLayer; b++) hiddenLayer.neurons.push_back(Neuron(numNeuronsPerHiddenLayer));
        net.push_back(hiddenLayer);
    }
    
    Layer outputLayer;
    for(int a = 0; a < numOutputs; a++) outputLayer.neurons.push_back(Neuron(numNeuronsPerHiddenLayer));
    net.push_back(outputLayer);
}

void NeuralNet::initWithStream(std::ifstream *input) {
	if(input->is_open()) {
		*input >> numInputs >> numOutputs >> numHiddenLayers >> numNeuronsPerHiddenLayer;

		std::string hiddenActivationFunctionName, outputActivationFunctionName;
		*input >> hiddenActivationFunctionName >> outputActivationFunctionName;
		setHiddenActivationFunction(hiddenActivationFunctionName);
		setOutputActivationFunction(outputActivationFunctionName);

		setupNeuronLayers();

		std::vector<double> newWeights;
		double middleMan = 0;
		while(*input >> middleMan) newWeights.push_back(middleMan);
		setWeights(newWeights);
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
            std::cout << "      Neuron " << b << ": ";
            for(int c = 0; c < net[a].neurons[b].weights.size(); c++) {
                std::cout << net[a].neurons[b].weights[c] << " ";
            }
            std::cout << "\n";
        }
    }
}