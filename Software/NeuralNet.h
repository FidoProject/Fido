#ifndef NEURALNET_H
#define NEURALNET_H

#include "Neuron.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <string>
#include <map>

namespace net {

	// Defines the type for an activation function for the neurons of a neural net
	typedef double(*ActivationFunction)(double);

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
		NeuralNet(int numInputs_, int numOutputs_, int numHiddenLayers_, int numNeuronsPerHiddenLayer_, std::string activationFunctionName);
		
		// Make a neural network with the same number of inputs, outputs, hidden layers, and neurons per hidden layer.
		NeuralNet(NeuralNet* otherNet);
        
        // Restores a neural network from the specified file
		NeuralNet(std::string filename);

		// Loads a neural network from a file using a stream
		NeuralNet(std::ifstream input);
        
        // Stores a neural network in the specified file
        void storeNet(std::string filename);

		// Stores a neural network using specified ofstream. Useful for appending a network to the end of a file without overwriting it. WARNING: will not close ofstream.
		void storeNetWithStream(std::ofstream &output);

		/* Get the weights of each neuron in the net.
		 * 
		 * These values are organized into a vector starting from the first neuron
		 * in the first layer to the last neuron in the output layer. 
		 */
		std::vector<double> getWeights();

		// Get the weights of each neuron in the net in 3d vector form
		std::vector< std::vector< std::vector<double> > > getWeights3D();

		/* Get the weights of each neuron in a hidden layer in a 2d vector format
		 *
		 * A hiddenLayerIndex value of 0 specifies the hidden layer that is directly fed the input and a value of numHiddenLayers-1 specifies the hidden layer closes to the ouput layer
		 */
		std::vector< std::vector<double> > getHiddenLayerWeights2D(int hiddenLayerIndex);

		// Get the weights of each neuron in the output layer in a 2d vector format
		std::vector< std::vector<double> > getOutputLayerWeights2D();
		
		/* Set the weights of each input for each neuron in the net according to a vector.
		 * 
		 */
		void setWeights(std::vector<double> w);

		// Set the weights of each input for each neuron in the net according to a 3d vector
		void setWeights3D(std::vector< std::vector< std::vector<double> > > w);
		
		/* Set the weights of each neuron in a hidden layer using a 2d vector format
		*
		* A hiddenLayerIndex value of 0 specifies the hidden layer that is directly fed the input and a value of numHiddenLayers-1 specifies the hidden layer closes to the ouput layer
		*/
		void setHiddenLayerWeights2D(int hiddenLayerIndex, std::vector< std::vector<double> > w);

		// Set the weights of each neuron in the output layer using a 2d vector format
		void setOutputLayerWeights2D(std::vector< std::vector<double> > w);

		/* Produce an output based on an input vector of doubles.
		 *
		 * The crowning function of this class.*/
		std::vector<double> getOutput(std::vector<double> input);

		// Returns the output of each layer of neurons as an input is fed for just as it is fed forward in getOutput
		std::vector< std::vector<double> > feedForward(std::vector<double> input);

		// Gets a map with the names of each activation function as its keys and the available activation functions as its values
		std::map<std::string, ActivationFunction> getActivationFunctionNameMap();

		// Sets the activation function for the neurons of the hidden layers with the name of the activation funciton
		void setHiddenActivationFunction(std::string name);

		// Sets the activation function for the neurons of the output layer with the name of the activation funciton
		void setOutputActivationFunction(std::string name);

		// Gets the name of the activation function of the nuerons of the hidden layers
		std::string getHiddenActivationFunctionName();

		// Gets the name of the activation function of the neurons of the output layer
		std::string getOutputActivationFunctionName();
		
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
            return floor(initialOutput);
		}

		// Takes the initial output of a neuron and returns it unchanged
		static double simpleLinear(double initialOutput) {
			return initialOutput;
		}

		// A two dimensional network of neurons.
		std::vector< std::vector<Neuron> > net;

		// Variables used to describe Neural Networks.
		int numInputs, numOutputs, numHiddenLayers, numNeuronsPerHiddenLayer;
		
    private:
		/* The activation function for the hidden neurons
		*
		* An activation function takes the summation of the inputs times the weights of a neuron and uses an activation value of 0 to filter the output
		*/
		ActivationFunction hiddenActivationFunction;

		/* The activation function for the output neurons
		*
		* An activation function takes the summation of the inputs times the weights of a neuron and uses an activation value of 0 to filter the output
		*/
		ActivationFunction outputActivationFunction;

        // Creates layers of neurons with random weights according to the values of numInputs, numOutputs, numHiddenLayers, and numNeuronsPerHiddenLayer
        void setupNeuronLayers();   
	};
}

#endif