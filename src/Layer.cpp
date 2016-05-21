#include "../include/Layer.h"

#include <assert.h>

using namespace net;

Layer::Layer() {
	activationFunction = sigmoid;
}

Layer::Layer(std::ifstream *input) {
	int numNeurons, numWeights;
	std::string activationFunctionName;
	*input >> numNeurons >> numWeights;
	*input >> activationFunctionName;

	setActivationFunctionWithName(activationFunctionName);

	for (int a = 0; a < numNeurons; a++) {
		std::vector<double> weights;
		for (int b = 0; b < numWeights; b++) {
			double weight;
			*input >> weight;
			weights.push_back(weight);
		}
		neurons.push_back(Neuron(weights));
	}
}

void Layer::setActivationFunctionWithName(std::string name) {
	std::map<std::string, ActivationFunction> nameMap = getActivationFunctionNameMap();
	std::map<std::string, ActivationFunction>::iterator nameIterator = nameMap.find(name);
	assert(nameIterator != nameMap.end());
	activationFunction = getActivationFunctionNameMap()[name];
}

std::string Layer::getActivationFunctionName() {
	std::map<std::string, ActivationFunction> nameMap = getActivationFunctionNameMap();
	for (std::map<std::string, ActivationFunction>::iterator a = nameMap.begin(); a != nameMap.end(); ++a) if (a->second == activationFunction) return a->first;
	assert(false);
}

void Layer::store(std::ofstream *output) {
	// All the neurons in the same layer have the same number of weights
	// so that is stored once along with the number of neurons in the layer
	// and the activation function name of the layer
	*output << neurons.size() << " " << neurons[0].weights.size() << " " << getActivationFunctionName() << "\n";
	for (unsigned int a = 0; a < neurons.size(); a++) {
		for (unsigned int b = 0; b < neurons[a].weights.size(); b++) {
			*output << neurons[a].weights[b] << " ";
		}
		*output << "\n";
	}
}
