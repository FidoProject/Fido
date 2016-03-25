#ifndef LAYER_H
#define LAYER_H

#include <map>
#include <fstream>
#include <string>
#include <iostream>

#include "Neuron.h"
#include "ActivationFunctions.h"

namespace net {
	/**
	 * \brief A layer in a NeuralNet.
	 */
	struct Layer {
		/** 
		 * \brief The activation function for the neurons of this layer
		 * 
		 * An activation function takes the summation of the inputs times the weights of a neuron and uses an activation value of 0 to filter the output
		 */
		ActivationFunction activationFunction;
		
		/** 
		 * \brief Holds the neurons of this layer
		 */
		std::vector<Neuron> neurons;

		/** 
		 * \brief Initialize a layer with the default sigmoid activation function. 
		 */
		Layer() {
			activationFunction = sigmoid;
		}

		/** 
		 * \brief Initialize a layer from a saved text file
		 *
		 * The saved text file must be formatted as outputted
		 * by the storeNetWithStream(std::ofstream *output) function.
		 *
		 * \param input the text file for the Layer object to be initialized from
		 */
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

		/** 
		 * \brief Gets the map of activation function name to ActivationFunction
		 */
		std::map<std::string, ActivationFunction> getActivationFunctionNameMap() {
			std::map<std::string, ActivationFunction> map;
			map["sigmoid"] = sigmoid;
			map["binary"] = binary;
			map["integer"] = integer;
			map["simpleLinear"] = simpleLinear;
			map["tanSigmoid"] = tanSigmoid;

			return map;
		}

		/** 
		 * \brief Sets the activation function to the given string
		 *
		 * \param name the name of the activation function
		 */
		void setActivationFunctionWithName(std::string name) {
			std::map<std::string, ActivationFunction> nameMap = getActivationFunctionNameMap();
			auto nameIterator = nameMap.find(name);
			if(nameIterator != nameMap.end()) {
				activationFunction = getActivationFunctionNameMap()[name];
			} else {
				std::cout << "The activation function \"" << name << "\" does not exist! Throwing error!\n";
				throw 1;
			}
		}


		/** 
		 * \brief Gets the name of the layer's activation function
		 */
		std::string getActivationFunctionName() {
			std::map<std::string, ActivationFunction> nameMap = getActivationFunctionNameMap();
			for (std::map<std::string, ActivationFunction>::iterator a = nameMap.begin(); a != nameMap.end(); ++a) if (a->second == activationFunction) return a->first;

			throw 1;
		}


		/** 
		 * \brief Store the layer to a text file
		 *
		 * This can later be read by the Layer(std::ifstream *input) constructor.
		 *
		 * \param output the text file for the layer object to be written to
		 */
		void store(std::ofstream *output) {
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
