#ifndef NEURALNET_H
#define NEURALNET_H

#include "Neuron.h"
#include <iostream>
#include <vector>
#include <math.h>

namespace net {

	/* Class representing a neural network.
	 *
	 * A flexible implementation designed to be usable in a wide scope of projects.
	 */
	class NeuralNet {
	public:

		// Makes an empty neural network
		NeuralNet();

		/* Manually initialize a neural network.
		 * 
		 * Uses the number of inputs, the number of outputs, the number of hidden layers, and the number of neurons per hidden layer.
		 */
		NeuralNet(int numInputs_, int numOutputs_, int numHiddenLayers_, int numNeuronsPerHiddenLayer_, double(*filterNeuronOutput_)(double initialOutput));
		
		// Make a neural network with the same number of inputs, outputs, hidden layers, and neurons per hidden layer.
		NeuralNet(NeuralNet const &otherNet);

		/* Get the weights of each neuron in the net.
		 * 
		 * These values are organized into a vector starting from the first neuron
		 * in the first layer to the last neuron in the output layer. 
		 */
		std::vector<double> getWeights();
		
		/* Set the weights of each neuron in the net according to a vector.
		 * 
		 * Starts at the first neuron in the first layer and goes to the last neruon in the last layer. 
		 */
		void setWeights(std::vector<double> w);

		/* Produce an output based on an input vector of doubles.
		 *
		 * The crowning function of this class.*/
		std::vector<double> getOutput(std::vector<double> input);
		
		/* An example of a common "s-shaped" function that takes the initial output of a neuron and uses an activation value of 0 to return a gradient output
		 *
		 *
		 */
		static double sigmoid(double initialOutput) {
			return ( 1 / ( 1 + exp(-initialOutput / 1)));
		}

		// An example of a common function that takes the initial output of a neuron and uses an activation value of 0 to return a binary output
		static double binary(double initialOutput) {
			return (initialOutput > 0 ? 1 : 0);
		}

		// An example of a common function that takes the initial output of a neuron and uses an activation value of 0 to return a binary output
		static double integer(double initialOutput) {
            
            if(initialOutput > 0) return ((long)initialOutput) % 3;
            else return 0;
		}

		// Takes the initial output of a neuron and uses an activation value of 0 to filter the output
		double(*filterNeuronOutput)(double initialOutput);

		// A two dimensional network of neurons.
		std::vector< std::vector<Neuron> > net;

		// Variables used to describe Neural Networks.
		int numInputs, numOutputs, numHiddenLayers, numNeuronsPerHiddenLayer;
	};
}

#endif