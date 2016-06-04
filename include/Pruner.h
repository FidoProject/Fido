#pragma once

#include <iostream>
#include <map>
#include <vector>

namespace net {
class NeuralNet;
class Trainer;

/** Removes unnecessary neurons from a NeuralNet. */
class Pruner {
public:
  /**
   * \brief Initialize the Pruner object.
   */
  Pruner();

  void pruneRandomnly(NeuralNet *network);

  void prune(NeuralNet *network, Trainer *networkTrainer);

  void prune(NeuralNet *network, const std::vector< std::vector< std::vector<double> > > &initialWeights, const std::vector< std::vector< std::vector< std::vector<double> > > > &weightChanges, const std::vector< std::vector< std::vector< std::vector<double> > > > &gradients);

};
}
