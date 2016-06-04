#pragma once

#include <vector>
#include <cstdlib>

namespace net {

/**
 * \brief A neuron in a Layer of a NeuralNet.
 *
 * Each neuron takes a variable number of inputs each assigned
 * a weight and returns one output.  Each neuron has a threshold
 * value used to determine if an output is produced (or to
 * determine the magnitude of the neuron's output). A quality class
 * by all accounts.
 */
class Neuron {
public:
  /**
   * \brief Set of weights for the Neuron object.
   */
  std::vector<double> weights;

  /**
   * \brief Constructs a neuron with randomly assigned weights.
   *
   * Each weight is a random double between 0 and 1.
   *
   * \param numInputs the number of inputs to the neuron
   */
  explicit Neuron(int numInputs);

  /**
   * \brief Constructs a neuron with given weights
   *
   * \param w the input weights from which to construct the neuron.
   */
  explicit Neuron(std::vector<double> w);

  /**
   * \brief Gets outputs of neuron from given inputs.
   *
   * The last weight is the threshold, as documented [here](http://www.ai-junkie.com/ann/evolved/nnt6.html).
   */
  double getOutput(std::vector<double> inputs);

  /**
   * \brief Randomizes the weights of the neuron.
   */
  void randomizeWeights();

  /**
   * \brief Generates a random input weight
   *
   * \return A random double between zero and one.
   */
  static double randomWeight() {
    return ((double)rand() / ((double)RAND_MAX / 2.0)) - 1.0;
  }
};
}
