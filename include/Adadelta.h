#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "SGDTrainer.h"
#include "ActivationFunctions.h"

namespace net {

class NeuralNet;

/** An adaptive learning rate Trainer (Zeiler). */
class Adadelta : public SGDTrainer {
public:
  /**
   * \brief Initialize an Adadelta object with necessary constants.
   * \param rho_ decay level of the trainer. Between 0 and 1.
   * \param targetErrorLevel_ at this error level, a net will be considered trained
   * \param maximumEpochs_ after this number of training iterations (one pass through all of the data points), a net will stop being trained no matter what
   */
  Adadelta(double rho_, double targetErrorLevel_, int maximumEpochs_);

  /**
   * \brief Stores an Adadelta object using specified ofstream.
   *
   * \param output pointer to the output stream which the neural network will be written to
  **/
  void store(std::ofstream *output);

  void initFromStream(std::ifstream *in);

  double rho; /**< the decay rate of the system, set by constructor */
  double epsilon; /**< a very small number used for root mean square calculations */

protected:
  std::vector< std::vector< std::vector<double> > > accumulatedGradients;
  std::vector< std::vector< std::vector<double> > > accumulatedUpdates;

  /** Resets the Adadelta object's accumulation vectors */
  void resetNetworkVectors(net::NeuralNet *network);

  double getChangeInWeight(double weight, int layerIndex, int neuronIndex, int weightIndex);
};
}
