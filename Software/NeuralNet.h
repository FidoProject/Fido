#ifndef NEURALNET_H
#define NEURALNET_H

#include "Neuron.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <string>

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
		NeuralNet(NeuralNet* otherNet);
        
        /* Restores a neural network from the specified file
         * 
         * The function for filtering neuron output may not be stored in the file currently, so must be passed in.
         */
        NeuralNet(std::string filename, double(*filterNeuronOutput_)(double initialOutput));
        
        // Stores a neural network in the specified file
        void storeNet(std::string filename);

		/* Get the weights of each neuron in the net.
		 * 
		 * These values are organized into a vector starting from the first neuron
		 * in the first layer to the last neuron in the output layer. 
		 */
		std::vector<double> getWeights();
		
		/* Set the weights of each input for each neuron in the net according to a vector.
		 * 
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

		// An example of a common "s-shaped" function that takes the initial output of a neuron and uses an activation value of 0 to return a gradient output
		static double sigmoidTicTacToe(double initialOutput) {
			return (int)(9 / (1 + exp(-initialOutput / 9.0)));
		}

		// An example of a common function that takes the initial output of a neuron and uses an activation value of 0 to return a binary output
		static double binary(double initialOutput) {
			return (initialOutput > 0 ? 1 : 0);
		}

		// An example of a common function that takes the initial output of a neuron and uses an activation value of 0 to return a binary output
		static double integer(double initialOutput) {
            
            if(initialOutput > 0) return ((long)initialOutput) % 9;
            else return 0;
		}

		// Takes the initial output of a neuron and returns it unchanged
		static double unchanged(double initialOutput) {
			return initialOutput;
		}

		// A two dimensional network of neurons.
		std::vector< std::vector<Neuron> > net;

		// Variables used to describe Neural Networks.
		int numInputs, numOutputs, numHiddenLayers, numNeuronsPerHiddenLayer;
    private:
        // Creates layers of neurons with random weights according to the values of numInputs, numOutputs, numHiddenLayers, and numNeuronsPerHiddenLayer
        void setupNeuronLayers();
        
        // Takes the initial output of a neuron and uses an activation value of 0 to filter the output
        double(*filterNeuronOutput)(double initialOutput);
	};
}

#endif