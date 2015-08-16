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

		/* Trains a neural network on a training set.
		 *
		 * Edits the weights of the neural network until its error in predicting the correctOutput of each input reaches the value of targetErrorLevel
		 * or the number of training cycles reaches the value of maximumIterations.
		 */
		void trainOnData(net::NeuralNet *network, std::vector< std::vector<double> > input, std::vector< std::vector<double> > correctOutput);

		// Gets the output of the neural network, calculates the error of each neuron, and edits the weights of the neurons to reduce error
		double trainOnDataPoint(net::NeuralNet *network, std::vector<double> input, std::vector<double> correctOutput);

		double learningRate, momentumTerm, targetErrorLevel;
		int maxiumumIterations;
		std::vector< std::vector< std::vector<double> > > lastChangeInWeight;

		static double sigmoidDerivative(double neuronOuput) {
			return neuronOuput * (1 - neuronOuput);
		}
		static double simpleLinearDerivative(double neuronOutput) {
			return 1;
		}

		double(*hiddenActivationFunctionDerivative)(double neuronOuput);
		double(*outputActivationFunctionDerivative)(double neuronOuput);
	};
}

#endif