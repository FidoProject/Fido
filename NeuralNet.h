#ifndef NEURALNET_H
#define NEURALNET_H

#include "Neuron.h"
#include <iostream>
#include <vector>
#include <math.h>

class NeuralNet {
public:
	std::vector< std::vector<Neuron> > net;

	NeuralNet(int numInputs, int numOutputs, int numHiddenLayers, int numNeruronsPerHiddenLayer);
	std::vector<double> getWeights();
	void setWeights(std::vector<double> w);
	std::vector<double> getOutput(std::vector<double> input);
	double sigmoid(double activiation);
};

#endif