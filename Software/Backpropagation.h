#ifndef BACKPROPAGATION_H
#define BACKPROPAGATION_H

#include <iostream>

#include "NeuralNet.h"

class Backpropogation {
public:
	// Initialize the Backpropagation object with the learningRate and momentumTerm constants
	Backpropogation(double learningRate_, double momentumTerm_, double targetErrorLevel_, int maximumIterations_);

	void trainOnData(net::NeuralNet *network, std::vector< std::vector<double> > input, std::vector< std::vector<double> > correctOutput);

	double trainOnDataPoint(net::NeuralNet *network, std::vector<double> input, std::vector<double> correctOutput);

	double learningRate, momentumTerm, targetErrorLevel;
	int maxiumumIterations;
};

#endif