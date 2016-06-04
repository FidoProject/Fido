#pragma once

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
  virtual double train(net::NeuralNet *network, const std::vector< std::vector<double> > &input, const std::vector< std::vector<double> > &correctOutput) = 0;

  /**
   * \brief Stores a Trainer object using specified stream.
   *
   * \param output pointer to the output stream which the neural network will be written to
  **/
  virtual void store(std::ofstream *output) = 0;

  std::vector< std::vector< std::vector< std::vector<double> > > > getGradients() {
    return gradients;
  };
  std::vector< std::vector< std::vector< std::vector<double> > > > getWeightChanges() {
    return weightChanges;
  };
  std::vector< std::vector< std::vector<double> > > getInitialWeights() {
    return initialWeights;
  };
  std::vector< std::vector< std::vector<double> > > getFinalWeights() {
    return finalWeights;
  };

protected:
  std::vector< std::vector< std::vector< std::vector<double> > > > gradients, weightChanges;
  std::vector< std::vector< std::vector<double> > > initialWeights, finalWeights;
};
}
