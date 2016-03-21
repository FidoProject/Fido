#ifndef NEURALNET_H
#define NEURALNET_H

#include <fstream>
#include <map>

#include "Layer.h"
#include "ActivationFunctions.h"

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
		NeuralNet(int numInputs, int numOutputs, int numHiddenLayers, int numNeuronsPerHiddenLayer, std::string activationFunctionName);
		
		// Make a neural network with random weights but the same number of inputs, outputs, hidden layers, and neurons per hidden layer as the network given.
		explicit NeuralNet(NeuralNet* otherNet);
		
		// Restores a neural network from the specified file
		explicit NeuralNet(std::string filename);

		// Loads a neural network from a file using a stream
		explicit NeuralNet(std::ifstream *input);
		
		// Stores a neural network in the specified file
		void storeNet(std::string filename);

		// Stores a neural network using specified ofstream. Useful for appending a network to the end of a file without overwriting it. WARNING: will not close ofstream.
		void storeNetWithStream(std::ofstream *output);

		/* Get the weights of each neuron in the net.
		 * 
		 * These values are organized into a vector starting from the first neuron
		 * in the first layer to the last neuron in the output layer. 
		 */
		std::vector<double> getWeights();

		// Get the weights of each neuron in the net in 3d vector form
		std::vector< std::vector< std::vector<double> > > getWeights3D();
		
		/* Set the weights of each input for each neuron in the net according to a vector.
		 * 
		 */
		void setWeights(std::vector<double> w);

		// Set the weights of each input for each neuron in the net according to a 3d vector
		void setWeights3D(std::vector< std::vector< std::vector<double> > > w);

		// Randomizes all the weights of the neurons in the net
		void randomizeWeights();

		/* Produce an output based on an input vector of doubles.
		 *
		 * The crowning function of this class.*/
		std::vector<double> getOutput(std::vector<double> input);

		// Returns the output of each layer of neurons as an input is fed for just as it is fed forward in getOutput
		std::vector< std::vector<double> > feedForward(std::vector<double> input);
		
		// Prints the weights of the neurons of the layers of the net
		void printWeights();
		
		// Gets the number of hidden layers in the neural net. Returns the number of layers minus 1
		int numberOfHiddenLayers();

		// Gets the number of intput neurons in the NeuralNet
		int numberOfInputs();

		// Gets the number of output neurons in the NeuralNet
		int numberOfOutputs();

		// Sets the activation function of the output layer
		void setOutputActivationFunction(std::string name);

		// A two dimensional network of neurons.
		std::vector< Layer > net;
		
	private:
		void initWithStream(std::ifstream *input);
	};
}

#endif