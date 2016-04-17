#include "../include/Backpropagation.h"

#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <string>

#include "../include/NeuralNet.h"

using namespace net;

Backpropagation::Backpropagation(double learningRate_, double momentumTerm_, double targetErrorLevel_, int maximumIterations_) {
	learningRate = learningRate_;
	momentumTerm = momentumTerm_;
	targetErrorLevel = targetErrorLevel_;
	maximumIterations = maximumIterations_;
}

Backpropagation::Backpropagation() {
	learningRate = 0.01;
	momentumTerm = 0.9;
	targetErrorLevel = 0.6;
	maximumIterations = 35000;
}

Backpropagation::Backpropagation(std::ifstream *input) {
	if(input->is_open()) {
		*input >> learningRate >> momentumTerm >> targetErrorLevel >> maximumIterations;
	} else {
		std::cout << "Could not retrieve backprop from file\n";
		throw 1;
	}
}

void Backpropagation::store(std::ofstream *output) {
	if(output->is_open()) {
		*output << learningRate << " " << momentumTerm << " " << targetErrorLevel << " " << maximumIterations << "\n";
	} else {
		std::cout << "Could not store backprop\n";
		throw 1;
	}
}

void Backpropagation::train(net::NeuralNet *network, const std::vector< std::vector<double> > &input, const std::vector< std::vector<double> > &correctOutput) {
	double totalError = 0;
	int iterations = 0;
	resetLastChangeInWeight(network);

	do {
		totalError = 0;
		for(unsigned int a = 0; a < input.size(); a++) {
			totalError += trainOnDataPoint(network, input[a], correctOutput[a]);
		}
		iterations++;
	} while(totalError > targetErrorLevel && iterations < maximumIterations);

	if(iterations >= maximumIterations-1) std::cout << "Backpropagation hit max iterations with an error level of " << totalError << ".\n";
}


double Backpropagation::trainOnDataPoint(net::NeuralNet *network, const std::vector<double> &input, const std::vector<double> &correctOutput) {
	std::vector< std::vector<double> > outputs = network->feedForward(input);
	std::vector< std::vector< std::vector<double> > > weights = network->getWeights3D();
	std::vector< std::vector<double> > errors = network->getGradients(input, correctOutput);

	// Update weights
	for(unsigned int errorIndex = 0; errorIndex < errors.size(); errorIndex++) {
		int layerIndex = ((int)errors.size() - 1) - errorIndex;

		for(unsigned int neuronIndex = 0; neuronIndex < errors[errorIndex].size(); neuronIndex++) {
			if(errorIndex == errors.size() - 1) {
				for(unsigned int inputIndex = 0; inputIndex < input.size(); inputIndex++) {
					double deltaWeight = learningRate * errors[errorIndex][neuronIndex] * input[inputIndex] + lastChangeInWeight[layerIndex][neuronIndex][inputIndex]*momentumTerm;
					weights[layerIndex][neuronIndex][inputIndex] += deltaWeight;
					lastChangeInWeight[layerIndex][neuronIndex][inputIndex] = deltaWeight;
				}
			} else {
				for(unsigned int previousOutput = 0; previousOutput < outputs[layerIndex - 1].size(); previousOutput++) {
					double deltaWeight = learningRate * errors[errorIndex][neuronIndex] * outputs[layerIndex - 1][previousOutput] + lastChangeInWeight[layerIndex][neuronIndex][previousOutput]*momentumTerm;
					weights[layerIndex][neuronIndex][previousOutput] += deltaWeight;
					lastChangeInWeight[layerIndex][neuronIndex][previousOutput] = deltaWeight;
				}
			}

			// Change the activation weight
			double deltaWeight = -learningRate * errors[errorIndex][neuronIndex] + lastChangeInWeight[layerIndex][neuronIndex][weights[layerIndex][neuronIndex].size() - 1]*momentumTerm;
			weights[layerIndex][neuronIndex][weights[layerIndex][neuronIndex].size() - 1] += deltaWeight;
			lastChangeInWeight[layerIndex][neuronIndex][weights[layerIndex][neuronIndex].size() - 1] = deltaWeight;
		}

	}

	network->setWeights3D(weights);

	double networkError = 0;
	// Compute network error
	for(unsigned int outputNeuronIndex = 0; outputNeuronIndex < outputs[outputs.size()-1].size(); outputNeuronIndex++) {
		networkError += pow(correctOutput[outputNeuronIndex] - outputs[outputs.size()-1][outputNeuronIndex], 2);
	}

	return networkError;
}



void Backpropagation::resetLastChangeInWeight(net::NeuralNet *network) {
	lastChangeInWeight.clear();
	std::vector< std::vector< std::vector<double> > > weights = network->getWeights3D();
	for(unsigned int a = 0; a < weights.size(); a++) {
		std::vector< std::vector<double> > layer;
		for(unsigned int b = 0; b < weights[a].size(); b++) {
			std::vector<double> neuron;
			for(unsigned int c = 0; c < weights[a][b].size(); c++) {
				neuron.push_back(0);
			}
			layer.push_back(neuron);
		}
		lastChangeInWeight.push_back(layer);
	}
}
