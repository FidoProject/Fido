#include "../include/Backpropagation.h"

#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>

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

void Backpropagation::store(std::ofstream *output) {
	assert(output->is_open());
	SGDTrainer::store(output);
	*output << learningRate << " " << momentumTerm << "\n";
}

void Backpropagation::initFromStream(std::ifstream *in) {
  assert(in->is_open());
	SGDTrainer::initFromStream(in);
	*in >> learningRate >> momentumTerm;
}

double Backpropagation::getChangeInWeight(double weight, int layerIndex, int neuronIndex, int weightIndex) {
	double lastWeight = weightChanges.size() == 0 ? 0 : weightChanges.back()[layerIndex][neuronIndex][weightIndex];
	return learningRate * gradients.back()[layerIndex][neuronIndex][weightIndex] + lastWeight*momentumTerm;
}
