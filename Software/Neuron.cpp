#include "Neuron.h"

#include <iostream>
#include <stdlib.h>

using namespace net;

Neuron::Neuron(int numInputs) {
	for(int a = 0; a < numInputs+1; a++) weights.push_back(Neuron::randomWeight());
}

Neuron::Neuron(std::vector<double> w) {
	weights = w;
}

void Neuron::randomizeWeights() {
	for (int a = 0; a < weights.size(); a++) weights[a] = Neuron::randomWeight();
}

double Neuron::getOutput(std::vector<double> inputs) {
	if(inputs.size() != weights.size() - 1) {
		std::cout << "Wrong number of neuron inputs\n";
		throw 1;
	}
	double returnNumber = 0;
	for(int a = 0; a < inputs.size(); a++) returnNumber += inputs[a]*weights[a];
	returnNumber += -1 * weights[weights.size()-1];
	return returnNumber;
}