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
	 * */
	class NeuralNet {
	public:
		// A two dimensional network of neurons.
		std::vector< std::vector<Neuron> > net;

		// Initialize a network using the number of inputs, the number of outputs, the number of hidden layers, and the number of neurons per hidden layer.
		NeuralNet(int numInputs_, int numOutputs_, int numHiddenLayers_, int numNeuronsPerHiddenLayer_);
		
		// Makes a neural network with the same number of inputs, outputs, hidden layers, and neurons per hidden layer.
		NeuralNet(NeuralNet otherNet);
		

		/* Gets the weights of each neuron in the net. O
		 * 
		 * These values are organized into a vector starting from the first neuron
		 * in the first layer to the last neuron in the output layer. */
		std::vector<double> getWeights();
		
		/* Sets the weights of each neuron in the net according to a vector.
		 * 
		 * Starts at the first neuron in the first layer and goes to the last neruon in the last layer. */
		void setWeights(std::vector<double> w);

		/* Produces an output based on an input vector of doubles.
		 *
		 * The crowning function of this class.*/
		std::vector<double> getOutput(std::vector<double> input);
		
		/* A s-shaped math function.
		 *
		 * Used by the network to get a gradient value for each neurons output. */
		double sigmoid(double activiation);

		// Variables used to describe Neural Networks.
		int numInputs, numOutputs, numHiddenLayers, numNeuronsPerHiddenLayer;
	};
}

#endif