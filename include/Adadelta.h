#ifndef ADADELTA_H
#define ADADELTA_H

#include <iostream>
#include <map>
#include <vector>

#include "Trainer.h"
#include "ActivationFunctions.h"

namespace net {

	class NeuralNet;

	/** A classic backpropagation SGD Trainer. */
	class Adadelta : public Trainer {
	public:
		/**
		 * \brief Initialize the Backpropagation object with necessary constants.
		 * \param learningRate_ controls how much the neural network is modified each learning iteration
		 * \param momentumTerm_ allows a network to escape a local max
		 * \param targetErrorLevel_ at this error level, a net will be considered trained
		 * \param maximumEpochs_ after this number of training iterations (one pass through all of the data points), a net will stop being trained no matter what
		 */
		Adadelta(double rho_, double targetErrorLevel_, int maximumEpochs_);

		/**
		 * \brief Trains a neural network on a training set until the target error level is reached.
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

    /**
		 * \brief Stores a Backpropagation object using specified ofstream.
		 *
		 * \param output pointer to the output stream which the neural network will be written to
		**/
		void store(std::ofstream *output);

		double targetErrorLevel; /**< The target error level, set by constructor */
		int maximumEpochs; /**< The maximum number of iterations, set by constructor */
    double rho, epsilon;
		std::vector< std::vector< std::vector<double> > > accumulatedGradients;
    std::vector< std::vector< std::vector<double> > > accumulatedUpdates;

	protected:

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
		void resetAccumulatedVectors(net::NeuralNet *network);


    double rms(double input);

	};
}

#endif
