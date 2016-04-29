#include "../include/Backpropagation.h"

#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <string>

#include "../include/NeuralNet.h"

using namespace net;

Backpropagation::Backpropagation(double learningRate_, double momentumTerm_, double targetErrorLevel_, int maximumEpochs_) {
	learningRate = learningRate_;
	momentumTerm = momentumTerm_;
	targetErrorLevel = targetErrorLevel_;
	maximumEpochs = maximumEpochs_;
}

Backpropagation::Backpropagation() {
	learningRate = 0.01;
	momentumTerm = 0.9;
	targetErrorLevel = 0.05;
	maximumEpochs = 10000;
}

Backpropagation::Backpropagation(std::ifstream *input) {
	if(input->is_open()) {
		*input >> learningRate >> momentumTerm >> targetErrorLevel >> maximumEpochs;
	} else {
		std::cout << "Could not retrieve backprop from file\n";
		throw 1;
	}
}

void Backpropagation::store(std::ofstream *output) {
	if(output->is_open()) {
		*output << learningRate << " " << momentumTerm << " " << targetErrorLevel << " " << maximumEpochs << "\n";
	} else {
		std::cout << "Could not store backprop\n";
		throw 1;
	}
}

void Backpropagation::train(net::NeuralNet *network, const std::vector< std::vector<double> > &input, const std::vector< std::vector<double> > &correctOutput) {
	double totalError = 0;
	int iterations = 0;
	resetNetworkVectors(network);

	do {
		totalError = 0;
		for(unsigned int a = 0; a < input.size(); a++) {
			totalError += trainOnDataPoint(network, input[a], correctOutput[a]);
		}
		iterations++;
	} while(totalError > targetErrorLevel && iterations < maximumEpochs);

	if(iterations >= maximumEpochs-1) std::cout << "Backpropagation hit max epochs with an error level of " << totalError << ".\n";

	finalWeights = network->getWeights3D();
}

double Backpropagation::trainEpocs(double numberOfEpochs, net::NeuralNet *network, const std::vector< std::vector<double> > &input, const std::vector< std::vector<double> > &correctOutput) {
	resetNetworkVectors(network);

	double totalError;
	for(int epochs = 0; epochs < numberOfEpochs; epochs++) {
		totalError = 0;
		for(unsigned int a = 0; a < input.size(); a++) {
			totalError += trainOnDataPoint(network, input[a], correctOutput[a]);
		}
	}

	finalWeights = network->getWeights3D();

	return totalError;
}


double Backpropagation::trainOnDataPoint(net::NeuralNet *network, const std::vector<double> &input, const std::vector<double> &correctOutput) {
	std::vector< std::vector< std::vector<double> > > weights = network->getWeights3D();
	gradients.push_back(network->getGradients(input, correctOutput));

	// Update weights
	std::vector< std::vector< std::vector<double> > > newWeightChanges(weights);
	for(unsigned int layerIndex = 0; layerIndex < weights.size(); layerIndex++) {
		for(unsigned int neuronIndex = 0; neuronIndex < weights[layerIndex].size(); neuronIndex++) {
			for(unsigned int weightIndex = 0; weightIndex < weights[layerIndex][neuronIndex].size(); weightIndex++) {
				double lastWeight = weightChanges.size() == 0 ? 0 : weightChanges.back()[layerIndex][neuronIndex][weightIndex];
				double deltaWeight = learningRate * gradients.back()[layerIndex][neuronIndex][weightIndex] + lastWeight*momentumTerm;
				weights[layerIndex][neuronIndex][weightIndex] += deltaWeight;
				newWeightChanges[layerIndex][neuronIndex][weightIndex] = deltaWeight;
			}
		}
	}
	weightChanges.push_back(newWeightChanges);

	network->setWeights3D(weights);

	double networkError = 0;
  std::vector<double> output = network->getOutput(input);
	for(unsigned int outputIndex = 0; outputIndex < output.size(); outputIndex++) {
		networkError += pow(correctOutput[outputIndex] - output[outputIndex], 2);
	}

	return networkError;
}



void Backpropagation::resetNetworkVectors(net::NeuralNet *network) {
	initialWeights = network->getWeights3D();

	weightChanges.clear();

	finalWeights.clear();
	gradients.clear();
}
