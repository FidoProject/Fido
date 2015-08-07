#include "Neuron.h"
using namespace net;

Neuron::Neuron(int numInputs) {
	for(int a = 0; a < numInputs; a++) weights.push_back(Neuron::randomWeight());
}

Neuron::Neuron(std::vector<double> w) {
	weights = w;
}

double Neuron::getOutput(std::vector<double> inputs) {
	double returnNumber;
	for(int a = 0; a < inputs.size(); a++) returnNumber += inputs[a]*weights[a];
	returnNumber += -1 * weights[weights.size()-1];
	return returnNumber;
}