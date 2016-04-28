#include "../include/Pruner.h"

#include <iostream>
#include <algorithm>
#include <math.h>
#include <climits>

#include "../include/NeuralNet.h"
#include "../include/Backpropagation.h"

using namespace net;

Pruner::Pruner() : Backpropagation() {
  pruningSampleSize = 200;
}

Pruner::Pruner(double learningRate_, double momentumTerm_, double targetErrorLevel_, unsigned int maximumEpochs_, unsigned int pruningSampleSize_) : Backpropagation(learningRate_, momentumTerm_, targetErrorLevel_, maximumEpochs_) {
  std::cout << "sample: " << pruningSampleSize_ << ", " << maximumEpochs << "\n";
  if(pruningSampleSize_ > maximumEpochs) {
    std::cout << "Error: pruning sample size greater than maximum epochs\n";
    throw 1;
  }

  pruningSampleSize = pruningSampleSize_;
}

Pruner::Pruner(std::ifstream *input) : Backpropagation(input) {
  if(input->is_open()) {
    *input >> pruningSampleSize;
	} else {
		std::cout << "Could not retrieve Pruner from stream.\n";
		throw 1;
	}
}

void Pruner::train(net::NeuralNet *network, const std::vector< std::vector<double> > &input, const std::vector< std::vector<double> > &correctOutput) {
  resetLastChangeInWeight(network);

  typedef std::vector< std::vector< std::vector<double> > > NeuralNetWeights;
  NeuralNetWeights initialWeights = network->getWeights3D();
  std::vector<NeuralNetWeights> deltaWeights;
  std::vector<Layer> lastNet = network->net;
  double lowestError = INT_MAX;

  // Train. Record weight changes.
  double totalError = INT_MAX;
  int epochs;
  for(epochs = 0; epochs < maximumEpochs && (epochs < pruningSampleSize*2 || totalError > targetErrorLevel); epochs++) {
    if(epochs != 0 && epochs % pruningSampleSize == 0) {
      // If lowered error, continue pruning
      if(totalError > lowestError-0.0001) {
        //std::cout << "Back up: " << totalError << "\n";
        network->net = lastNet;
      } else if(maximumEpochs - epochs > pruningSampleSize) { // If raised error, revert
        //std::cout << "Lowered error to " << totalError << "\n";
        lowestError = totalError;
        lastNet = network->net;
        std::cout << "Num neurons " << network->numberOfHiddenNeurons() << "\n";

        if(network->numberOfHiddenNeurons() > 4) {
          prune(network, initialWeights, deltaWeights, 1+int(network->numberOfHiddenNeurons()*0.25*(double(rand()) / double(RAND_MAX))));
        }
      }

      initialWeights = network->getWeights3D();
      deltaWeights.clear();
    }

    totalError = 0;
    for(unsigned int a = 0; a < input.size(); a++) {
      totalError += trainOnDataPoint(network, input[a], correctOutput[a]);
      deltaWeights.push_back(lastChangeInWeight);
    }
  }

  if(epochs >= maximumEpochs-1) std::cout << "Pruner has hit max epochs with an error level of " << totalError << ".\n";
}

void Pruner::prune(NeuralNet *network, const std::vector< std::vector< std::vector<double> > > &initialWeights, const std::vector< std::vector< std::vector< std::vector<double> > > > &weightChanges, unsigned int numNeuronsToPrune) {
  std::vector< std::vector< std::vector<double> > > finalWeights = network->getWeights3D();


  std::cout << "Prune " << numNeuronsToPrune << "\n";

  std::vector<std::vector<double> > sensitivities;

  // Get sensitivities of all neurons
  for(unsigned int a = 0; a < network->numberOfHiddenLayers(); a++) {
    std::vector<double> layerSensitivities;
    for(unsigned int b = 0; b < network->net[a].neurons.size(); b++) {
      double sensitivity = 0;

      for(unsigned int c = 0; c < network->net[a].neurons[b].weights.size(); c++) {
        if(finalWeights[a][b][c]-initialWeights[a][b][c] < 0.001) {
          std::for_each(weightChanges.begin(), weightChanges.end(), [&] (const std::vector< std::vector< std::vector<double> > > &weightChange) {
            sensitivity += fabs(pow(weightChange[a][b][c], 2) * finalWeights[a][b][c]/(learningRate * (finalWeights[a][b][c]-initialWeights[a][b][c])));
          });
        } else {
          sensitivity += 10000;
        }
      }

      sensitivity /= weightChanges.size()*network->net[a].neurons[b].weights.size();
      layerSensitivities.push_back(sensitivity);
    }
    sensitivities.push_back(layerSensitivities);
  }

  for(unsigned int numPruned = 0; numPruned < numNeuronsToPrune; numPruned++) {
    double lowestSensitivity = INT_MAX;
    int hiddenLayerIndex = 0, neuronIndex = 0;
    for(unsigned int a = 0; a < network->numberOfHiddenLayers(); a++) {
      for(unsigned int b = 0; b < network->net[a].neurons.size(); b++) {
        if(network->net[a].neurons.size() > 1 && sensitivities[a][b] < lowestSensitivity) {
          lowestSensitivity = sensitivities[a][b];
          hiddenLayerIndex = a;
          neuronIndex = b;
        }
      }
    }

    network->removeNeuron(hiddenLayerIndex, neuronIndex);
    sensitivities[hiddenLayerIndex].erase(sensitivities[hiddenLayerIndex].begin() + neuronIndex);
  }
}

void Pruner::store(std::ofstream *output) {
  if(output->is_open()) {
    Backpropagation::store(output);
    *output << pruningSampleSize;
  } else {
		std::cout << "Could not store Pruner. The output stream is not open.\n";
		throw 1;
	}
}
