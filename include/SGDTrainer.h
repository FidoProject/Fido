#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "Trainer.h"
#include "ActivationFunctions.h"

namespace net {

class NeuralNet;

/** A classic backpropagation SGD Trainer. */
class SGDTrainer : public Trainer {
public:

  /**
   * \brief Initialize empty Backpropagation object
   */
  SGDTrainer();

  /**
   * \brief Initialize the object with necessary constants.
   * \param targetErrorLevel_ at this error level, a net will be considered trained
   * \param maximumEpochs_ after this number of training iterations (one pass through all of the data points), a net will stop being trained no matter what
   */
  SGDTrainer(double targetErrorLevel_, int maximumEpochs_);

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
  double train(net::NeuralNet *network, const std::vector< std::vector<double> > &input, const std::vector< std::vector<double> > &correctOutput);

  /**
   * \brief Trains a neural network on a training set for a specified number of epochs.
   *
   * Edits the weights of the neural network until its error in predicting the correctOutput of each input reaches the value of targetErrorLevel
   * or the number of training cycles reaches the value of maximumIterations.
   * NOTE: If learning rate is not low enough, the weights of the neural network may got to infinity due to the nature of backpropagation.
   *
   * \param numberOfEpochs the number of training passes that will be made through the data
   * \param network the neural network to be trained
   * \param input a vector of neural network inputs; each element in input, should have a corresponding output in correctOutput
   * \param correctOutput network is trained to output an element of correctOutput when fed a corresponding element of the input vector
   */
  double trainEpocs(double numberOfEpochs, net::NeuralNet *network, const std::vector< std::vector<double> > &input, const std::vector< std::vector<double> > &correctOutput);

  void store(std::ofstream *out);

  void initFromStream(std::ifstream *in);


  double targetErrorLevel; /**< The target error level, set by constructor */
  int maximumEpochs; /**< The maximum number of iterations, set by constructor */

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
   * \brief Resets the Backpropagation object's neural network specific vectors using a neural network (NN is needed because the number of layers, neurons, and weights are needed).
   */
  virtual void resetNetworkVectors(net::NeuralNet *network);

  virtual double getChangeInWeight(double weight, int layerIndex, int neuronIndex, int weightIndex) = 0;
};
}
