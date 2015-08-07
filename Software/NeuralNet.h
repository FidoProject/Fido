#ifndef NEURALNET_H
#define NEURALNET_H

#include "Neuron.h"
#include <iostream>
#include <vector>
#include <math.h>

namespace net {
	class NeuralNet {
	public:
		std::vector< std::vector<Neuron> > net;

		NeuralNet(int numInputs_, int numOutputs_, int numHiddenLayers_, int numNeruronsPerHiddenLayer_);
		NeuralNet(NeuralNet otherNet);
		std::vector<double> getWeights();
		void setWeights(std::vector<double> w);
		std::vector<double> getOutput(std::vector<double> input);
		double sigmoid(double activiation);

		int numInputs, numOutputs, numHiddenLayers, numNeruronsPerHiddenLayer;
	};
}

#endif