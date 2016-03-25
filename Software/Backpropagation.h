#ifndef BACKPROPAGATION_H
#define BACKPROPAGATION_H

#include <iostream>
#include <map>
#include <vector>

#include "Trainer.h"
#include "ActivationFunctions.h"

namespace net {

	class NeuralNet;

	class Backpropagation : public Trainer {
	public:
		/**
		 * \brief Initialize the Backpropagation object with necessary constants.
		 * \param learningRate_ controls how much the neural network is modified each learning iteration
		 * \param momentumTerm_ allows a network to escape a local max
		 * \param targetErrorLevel_ at this error level, a net will be considered trained
		 * \param maxIterations_ after this number of training iterations, a net will stop being trained no matter what
		 */
		Backpropagation(double learningRate_, double momentumTerm_, double targetErrorLevel_, int maximumIterations_);

		/**
		 * \brief Initialize empty Backpropagation object
		 */
		Backpropagation();

		/**
		 * \brief Loads a Backpropagation object using an input stream
		 * \param input a pointer to the input stream that you want to form a neural network from
		 */
		explicit Backpropagation(std::ifstream *input);

		/**
		 * \brief Stores a Backpropagation object using specified ofstream.
		 *
		 * Useful for appending a network to the end of a file without overwriting it. WARNING: will not close ofstream.
		 *
		 * \param output pointer to the output stream which the neural network will be written to
		**/
		void store(std::ofstream *output);

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
		void train(net::NeuralNet *network, const std::vector< std::vector<double> > &input, const std::vector< std::vector<double> > &correctOutput);

		double learningRate; /**< The rate of learning, set by constructor */
		double momentumTerm; /**< The term of momentum, set by constructor */
		double targetErrorLevel; /**< The target error level, set by constructor */
		int maximumIterations; /**< The maximum number of iterations, set by constructor */
		std::vector< std::vector< std::vector<double> > > lastChangeInWeight; /**< The last change in weights */

	private:

		/**
		 * \brief Gets the output of the neural network, calculates the error of each neuron, and edits the weights of the neurons to reduce error
		 *
		 * \param network the neural network to be trained
		 * \param input the input fed to the neural network
		 * \param correctOutput network is trained to output this when fed the input vector
		 */
		double trainOnDataPoint(net::NeuralNet *network, const std::vector<double> &input, const std::vector<double> &correctOutput);

		/**
		 * \brief Resets the Backpropagation object's lastchanginweight instance variable using a neural network (NN is needed because the number of layers, neurons, and weights are needed).
		 */
		void resetLastChangeInWeight(net::NeuralNet *network);

		/**
		 * \brief Returns a map of derived activation function names to activation functions.
		 */
		std::map<std::string, ActivationFunction> getDerivedActivationFunctionNames();
	};
}

#endif
