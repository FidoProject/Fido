#ifndef LAYER_H
#define LAYER_H

#include <map>

#include "Neuron.h"
#include "ActivationFunctions.h"

namespace net {
	// A layer in a NeuralNet
	struct Layer {
		/* The activation function for the neurons of this layer
		*
		* An activation function takes the summation of the inputs times the weights of a neuron and uses an activation value of 0 to filter the output
		*/
		ActivationFunction activationFunction;
		
		// Holds the neurons of this layer
		std::vector<Neuron> neurons;

		std::map<std::string, ActivationFunction> getActivationFunctionNameMap() {
			std::map<std::string, ActivationFunction> map;
			map["sigmoid"] = sigmoid;
			map["binary"] = binary;
			map["integer"] = integer;
			map["simpleLinear"] = simpleLinear;
			map["tanSigmoid"] = tanSigmoid;

			return map;
		}

		void setActivationFunctionWithName(std::string name) {
			activationFunction = getActivationFunctionNameMap()[name];
		}

		std::string getActivationFunctionName() {
			std::map<std::string, ActivationFunction> nameMap = getActivationFunctionNameMap();
			for (auto a = nameMap.begin(); a != nameMap.end(); ++a) if (a->second == activationFunction) return a->first;

			throw 1;
		}
	};
}

#endif