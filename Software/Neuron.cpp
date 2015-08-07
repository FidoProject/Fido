#include "Neuron.h"
using namespace net;
// Creates a neuron with randomly assigned weights
Neuron::Neuron(int numInputs) {
	// Each weight is a random double between 0 and 1
	for(int a = 0; a < numInputs; a++) weights.push_back(Neuron::randomWeight());
}

Neuron::Neuron(std::vector<double> w) {
	weights = w;
}

// Neuron sums the inputs times each input's respective output
// The last weight does not have an input, but instead is the "threshold value"
double Neuron::getOutput(std::vector<double> inputs) {
	double returnNumber;
	for(int a = 0; a < inputs.size(); a++) returnNumber += inputs[a]*weights[a];

	// The last weight it the threshold
	// http://www.ai-junkie.com/ann/evolved/nnt6.html
	returnNumber += -1 + weights[weights.size()-1];

	return returnNumber;
}