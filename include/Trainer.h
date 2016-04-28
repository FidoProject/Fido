#ifndef TRAINER_H
#define TRAINER_H

#include <vector>
#include <fstream>

namespace net {

	class NeuralNet;

	/** Trains neural networks */
	class Trainer {
	public:
		/**
		 * \brief Trains a neural network on a training set.
		 *
		 * Edits the weights of the neural network until its error in predicting the correctOutput of each input reaches the value of targetErrorLevel
		 * or the number of training cycles reaches the value of maximumIterations.
		 *
		 * \param network the neural network to be trained
		 * \param input a vector of neural network inputs; each element in input, should have a corresponding output in correctOutput
		 * \param correctOutput network is trained to output an element of correctOutput when fed a corresponding element of the input vector
		 */
		virtual void train(net::NeuralNet *network, const std::vector< std::vector<double> > &input, const std::vector< std::vector<double> > &correctOutput) = 0;
	};
}

#endif
