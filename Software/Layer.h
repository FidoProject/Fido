#ifndef LAYER_H
#define LAYER_H

#include <map>
#include <fstream>
#include <string>

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

		Layer() {
			activationFunction = sigmoid;
		}

		explicit Layer(std::ifstream *input) {
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

		void storeNetWithStream(std::ofstream *output) {
			// All the neurons in the same layer have the same number of weights 
			// so that is stored once along with the number of neurons in the layer
			// and the activation function name of the layer
			*output << neurons.size() << " " << neurons[0].weights.size() << " " << getActivationFunctionName() << "\n";
			for (int a = 0; a < neurons.size(); a++) {
				for (int b = 0; b < neurons[a].weights.size(); b++) {
					*output << neurons[a].weights[b] << " ";
				}
				*output << "\n";
			}
		}
	};
}

#endif