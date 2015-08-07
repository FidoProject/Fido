#include "Neuron.h"

Neuron::Neuron(int numInputs) {
	// Each weight is a random double between 0 and 1
	for(int a = 0; a < numInputs; a++) weights.push_back(rand() / RAND_MAX);
}

Neuron::Neuron(std::vector<double> w) {
	weights = w;
}

double Neuron::getOutput(std::vector<double> inputs) {
	double returnNumber;
	for(int a = 0; a < inputs.size(); a++) returnNumber += inputs[a]*weights[a];

	// The last weight it the threshold
	// http://www.ai-junkie.com/ann/evolved/nnt6.html
	returnNumber += -1 + weights[weights.size()-1];

	return returnNumber;
}