#ifndef BACKPROPAGATION_H
#define BACKPROPAGATION_H

#include <iostream>
#include <map>
#include <vector>

#include "ActivationFunctions.h"

namespace net {

	class NeuralNet;

	class Backpropagation {
	public:
		/**
		 * \brief Initialize the Backpropagation object with necessary constants.
		 * \param learningRate_ controls how much the neural network is modified each learning iteration
		 * \param momentumTerm_ allows a network to escape a local max
		 * \param targetErrorLevel_ at this error level, a net will be considered trained
		 * \param maxIterations_ after this number of training iterations, a net will stop being trained no matter what
		 */
		Backpropagation(double learningRate_, double momentumTerm_, double targetErrorLevel_, int maximumIterations_);
		
		///
		//! Initialize empty Backpropagation object
		///
		Backpropagation();

		///
		//! Loads a Backpropagation object from a file
		///
		explicit Backpropagation(std::string filename);

		///
		//! Loads a Backpropagation object using an input stream
		///
		explicit Backpropagation(std::ifstream *input);

		///
		//! Stores a Backpropagation object in the specified file
		///
		void storeBackpropagation(std::string filename);

		///
		//! Stores a Backpropagation object using specified ofstream. Useful for appending a network to the end of a file without overwriting it. WARNING: will not close ofstream.
		///
		void storeBackpropagationWithStream(std::ofstream *output);

		/** 
		 * \brief Trains a neural network on a training set.
		 *
		 * Edits the weights of the neural network until its error in predicting the correctOutput of each input reaches the value of targetErrorLevel 
		 * or the number of training cycles reaches the value of maximumIterations.
		 * NOTE: If learning rate is not low enough, the weights of the neural network may got to infinity due to the nature of backpropagation.
		 *
		 * \param network the neural network to be trained
		 * \param input a vector of neural network inputs; each element in input, should have a corresponding output in correctOutput
		 * \param correctOutput network is trained to output an element of correctOutput when fed a corresponding element of the input vector
		 */
		void trainOnData(net::NeuralNet *network, const std::vector< std::vector<double> > &input, const std::vector< std::vector<double> > &correctOutput);

		//! Gets the output of the neural network, calculates the error of each neuron, and edits the weights of the neurons to reduce error
		/**
		 * \param network the neural network to be trained
		 * \param input the input fed to the neural network
		 * \param correctOutput network is trained to output this when fed the input vector
		 */
		double trainOnDataPoint(net::NeuralNet *network, const std::vector<double> &input, const std::vector<double> &correctOutput);

		///
		//! Gets a map with the names of each activation function derivative as its keys and the available activation function derivatives as its values
		///
		std::map<std::string, ActivationFunction> getDerivedActivationFunctionNames();

		///
		//! Sets the derivative of the activation function for the neurons of the hidden layers with the name of the activation funciton
		///
		void setDerivedHiddenActivationFunction(std::string name);

		///
		//! Sets the derivative of the activation function for the neurons of the output layer with the name of the activation funciton
		///
		void setDerivedOutputActivationFunction(std::string name);

		///
		//! Gets the name of the derivative of the activation function of the nuerons of the hidden layers
		///
		std::string getDerivedHiddenActivationFunctionName();

		///
		//! Gets the name of the derivative of the activation function of the neurons of the output layer
		///
		std::string getDerivedOutputActivationFunctionName();

		double learningRate, momentumTerm, targetErrorLevel;
		int maxiumumIterations;
		std::vector< std::vector< std::vector<double> > > lastChangeInWeight;

	private:
		ActivationFunction hiddenActivationFunctionDerivative;
		ActivationFunction outputActivationFunctionDerivative;

		///
		//! Uses a ifstream to initialize a backpropagation object
		///
		void initWithStream(std::ifstream *input);
        
        ///
        //! Resets the lastchanginweight vector using a neural network is needed (NN is needed cause the number of layers, neurons, and weights are needed).
        ///
        void resetLastChangeInWeight(net::NeuralNet *network);
	};
}

#endif