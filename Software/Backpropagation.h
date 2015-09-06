#ifndef BACKPROPAGATION_H
#define BACKPROPAGATION_H

#include <iostream>
#include <chrono>
#include <thread>

#include "NeuralNet.h"

namespace net {

	class Backpropagation {
	public:
		// Initialize the Backpropagation object with the learningRate and momentumTerm constants
		Backpropagation(double learningRate_, double momentumTerm_, double targetErrorLevel_, int maximumIterations_);
		
		// Initialize empty Backpropagation object
		Backpropagation();

		// Loads a Backpropagation object from a file
		Backpropagation(std::string filename);

		// Loads a Backpropagation object using an input stream
		Backpropagation(std::ifstream &input);

		// Stores a Backpropagation object in the specified file
		void storeBackpropagation(std::string filename);

		// Stores a Backpropagation object using specified ofstream. Useful for appending a network to the end of a file without overwriting it. WARNING: will not close ofstream.
		void storeBackpropagationWithStream(std::ofstream &output);

		/* Trains a neural network on a training set.
		 *
		 * Edits the weights of the neural network until its error in predicting the correctOutput of each input reaches the value of targetErrorLevel
		 * or the number of training cycles reaches the value of maximumIterations.
		 * NOTE: If learning rate is not low enough, the weights of the neurl network may got to infinity due to the nature of backpropagation.
		 */
		void trainOnData(net::NeuralNet *network, std::vector< std::vector<double> > input, std::vector< std::vector<double> > correctOutput);

		// Gets the output of the neural network, calculates the error of each neuron, and edits the weights of the neurons to reduce error
		double trainOnDataPoint(net::NeuralNet *network, const std::vector<double> &input, const std::vector<double> &correctOutput);

		// Gets a map with the names of each activation function derivative as its keys and the available activation function derivatives as its values
		std::map<std::string, ActivationFunction> getDerivedActivationFunctionNames();

		// Sets the derivative of the activation function for the neurons of the hidden layers with the name of the activation funciton
		void setDerivedHiddenActivationFunction(std::string name);

		// Sets the derivative of the activation function for the neurons of the output layer with the name of the activation funciton
		void setDerivedOutputActivationFunction(std::string name);

		// Gets the name of the derivative of the activation function of the nuerons of the hidden layers
		std::string getDerivedHiddenActivationFunctionName();

		// Gets the name of the derivative of the activation function of the neurons of the output layer
		std::string getDerivedOutputActivationFunctionName();

		double learningRate, momentumTerm, targetErrorLevel;
		int maxiumumIterations;
		std::vector< std::vector< std::vector<double> > > lastChangeInWeight;

	private:
		ActivationFunction hiddenActivationFunctionDerivative;
		ActivationFunction outputActivationFunctionDerivative;

		// Uses a ifstream to initialize a backpropagation object
		void initWithStream(std::ifstream &input);
        
        // Resets the lastchanginweight vector using a neural network is needed (NN is needed cause the number of layers, neurons, and weights are needed). 
        void resetLastChangeInWeight(net::NeuralNet *network);
	};
}

#endif