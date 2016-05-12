#include "../include/Backpropagation.h"

#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "../include/NeuralNet.h"

using namespace net;

Backpropagation::Backpropagation(double learningRate_, double momentumTerm_, double targetErrorLevel_, int maximumEpochs_) : SGDTrainer(targetErrorLevel_, maximumEpochs_) {
	learningRate = learningRate_;
	momentumTerm = momentumTerm_;
}

Backpropagation::Backpropagation() {
	learningRate = 0.01;
	momentumTerm = 0.9;
	targetErrorLevel = 0.05;
	maximumEpochs = 10000;
}

Backpropagation::Backpropagation(std::ifstream *input) {
	initFromStream(input);
}

bool Backpropagation::initFromStream(std::ifstream *in) {
	if(in->is_open()) {
		std::string name;
		*in >> name;
		if (name != "BackProp") {
			return false;
		}
		*in >> learningRate >> momentumTerm >> targetErrorLevel >> maximumEpochs;
		return true;
	} else {
		std::cout << "Could not retrieve backprop from file\n";
		throw 1;
	}
}

void Backpropagation::store(std::ofstream *output) {
	if(output->is_open()) {
		*output << "BackProp " << learningRate << " " << momentumTerm << "\n";
	} else {
		std::cout << "Could not store backprop\n";
		throw 1;
	}
}

bool Backpropagation::initFromStream(std::ifstream *in) {
  if(in->is_open()) {
		assert(SGDTrainer::initFromStream(in) == true);
		*in >> learningRate >> momentumTerm;
		return true;
	} else {
		std::cout << "Could not retrieve sgdtrainer from file\n";
		throw 1;
	}
}

double Backpropagation::getChangeInWeight(double weight, int layerIndex, int neuronIndex, int weightIndex) {
	double lastWeight = weightChanges.size() == 0 ? 0 : weightChanges.back()[layerIndex][neuronIndex][weightIndex];
	return learningRate * gradients.back()[layerIndex][neuronIndex][weightIndex] + lastWeight*momentumTerm;
}
