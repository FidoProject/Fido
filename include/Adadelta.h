#ifndef ADADELTA_H
#define ADADELTA_H

#include <iostream>
#include <map>
#include <vector>

#include "Trainer.h"
#include "ActivationFunctions.h"

namespace net {

	class NeuralNet;

	/** An adaptive learning rate Trainer (Zeiler). */
	class Adadelta : public Trainer {
	public:
		/**
		 * \brief Initialize an Adadelta object with necessary constants.
		 * \param rho_ decay level of the trainer. Between 0 and 1.
		 * \param targetErrorLevel_ at this error level, a net will be considered trained
		 * \param maximumEpochs_ after this number of training iterations (one pass through all of the data points), a net will stop being trained no matter what
		 */
		Adadelta(double rho_, double targetErrorLevel_, int maximumEpochs_);

		/**
		 * \brief Trains a neural network on a training set until the target error level is reached.
		 *
		 * Edits the weights of the neural network until its error in predicting the correctOutput of each input reaches the value of targetErrorLevel
		 * or the number of training cycles reaches the value of maximumIterations.
		 *
		 * \param network the neural network to be trained
		 * \param input a vector of neural network inputs; each element in input, should have a corresponding output in correctOutput
		 * \param correctOutput network is trained to output an element of correctOutput when fed a corresponding element of the input vector
		 */
		void train(net::NeuralNet *network, const std::vector< std::vector<double> > &input, const std::vector< std::vector<double> > &correctOutput);


		double targetErrorLevel; /**< The target error level, set by constructor */
		int maximumEpochs; /**< The maximum number of epochs for one training run, set by constructor */
    double rho; /**< the decay rate of the system, set by constructor */
		double epsilon; /**< a very small number used for root mean square calculations */

	private:
		std::vector< std::vector< std::vector<double> > > accumulatedGradients;
    std::vector< std::vector< std::vector<double> > > accumulatedUpdates;

		/**
		 * \brief Gets the output of the neural network, calculates the error of each neuron, and edits the weights of the neurons to reduce error
		 *
		 * \param network the neural network to be trained
		 * \param input the input fed to the neural network
		 * \param correctOutput network is trained to output this when fed the input vector
		 */
		double trainOnDataPoint(net::NeuralNet *network, const std::vector<double> &input, const std::vector<double> &correctOutput);

		void prune(NeuralNet *network, const std::vector< std::vector< std::vector<double> > > &initialWeights, const std::vector< std::vector< std::vector< std::vector<double> > > > &weightChanges, unsigned int numNeuronsToPrune);

		/** Resets the Adadelta object's accumulation vectors */
		void resetAccumulatedVectors(net::NeuralNet *network);

	};
}

#endif
