#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "SGDTrainer.h"
#include "ActivationFunctions.h"

namespace net {

class NeuralNet;

/** A classic SGDTrainer. */
class Backpropagation : public SGDTrainer {
public:
  /**
   * \brief Initialize the Backpropagation object with necessary constants.
   * \param learningRate_ controls how much the neural network is modified each learning iteration
   * \param momentumTerm_ allows a network to escape a local max
   * \param targetErrorLevel_ at this error level, a net will be considered trained
   * \param maximumEpochs_ after this number of training iterations (one pass through all of the data points), a net will stop being trained no matter what
   */
  Backpropagation(double learningRate_, double momentumTerm_, double targetErrorLevel_, int maximumEpochs_);

  /**
   * \brief Initialize empty Backpropagation object
   */
  Backpropagation();

  /**
   * \brief Loads a Backpropagation object using an input stream
   * \param input a pointer to the input stream that contains a stored Backpropagation model
   */
  explicit Backpropagation(std::ifstream *input);

  /**
   * \brief Stores a Backpropagation object using specified ofstream.
   *
   * \param output pointer to the output stream which the neural network will be written to
  **/
  void store(std::ofstream *output);

  void initFromStream(std::ifstream *in);

  double learningRate; /**< The rate of learning, set by constructor */
  double momentumTerm; /**< The term of momentum, set by constructor */

protected:
  double getChangeInWeight(double weight, int layerIndex, int neuronIndex, int weightIndex);
};
}
