#ifndef PRUNER_H
#define PRUNER_H

#include <iostream>
#include <map>
#include <vector>

#include "Trainer.h"
#include "Backpropagation.h"
#include "ActivationFunctions.h"

namespace net {

	class NeuralNet;

	/** A Trainer that couples backpropagation with a pruning algorithm (Karnin) to eliminate, unneccessary neurons  */
	class Pruner : public Backpropagation {
	public:
		/**
		 * \brief Initialize the Pruner object with default constants.
		 */
    Pruner();

		/** Initialize the Pruner object with necessary constants.
		 *
		 * \param learningRate_ controls how much the neural network is modified each learning iteration
		 * \param momentumTerm_ allows a network to escape a local max
		 * \param targetErrorLevel_ at this error level, a net will be considered trained
		 * \param maximumEpochs_ after this number of training iterations (one pass through all of the data points), a net will stop being trained no matter what
		 * \param pruningSampleSize_ the number of training epochs performed between trying to resize the network
		 */
    Pruner(double learningRate_, double momentumTerm_, double targetErrorLevel_, unsigned int maximumEpochs_, unsigned int pruningSampleSize_);

		/**
		 * \brief Loads a Pruner object using an input stream
		 * \param input a pointer to the input stream that you want that contains a stored Pruner
		 */
		explicit Pruner(std::ifstream *input);

		/**
		 * \brief Trains a neural network on a training set until the target error level is reached by modifying the weights and structure of the neural network.
		 *
		 * Edits the weights and sturcture of the neural network until its error in predicting the correctOutput of each input reaches the value of targetErrorLevel
		 * or the number of training cycles reaches the value of maximumIterations.
		 * NOTE: If learning rate is not low enough, the weights of the neural network may got to infinity due to the nature of backpropagation.
		 *
		 * \param network the neural network to be trained
		 * \param input a vector of neural network inputs; each element in input, should have a corresponding output in correctOutput
		 * \param correctOutput network is trained to output an element of correctOutput when fed a corresponding element of the input vector
		 */
    void train(net::NeuralNet *network, const std::vector< std::vector<double> > &input, const std::vector< std::vector<double> > &correctOutput);

		/**
		 * \brief Stores a Pruner in a stream.
     *
		 * \param output the stream that the pruner will be stored in.
		 */
    void store(std::ofstream *output);

	protected:
		int pruningSampleSize;
    Backpropagation trainer;

    void prune(NeuralNet *network, const std::vector< std::vector< std::vector<double> > > &initialWeights, const std::vector< std::vector< std::vector< std::vector<double> > > > &weightChanges, unsigned int numNeuronsToPrune);
	};
}

#endif

