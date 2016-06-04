#include "../include/SGDTrainer.h"

#include <assert.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <string>

#include "../include/NeuralNet.h"

using namespace net;

SGDTrainer::SGDTrainer() {
  targetErrorLevel = 0.01;
  maximumEpochs = 10000;
}

SGDTrainer::SGDTrainer(double targetErrorLevel_, int maximumEpochs_) {
  targetErrorLevel = targetErrorLevel_;
  maximumEpochs = maximumEpochs_;
}

double SGDTrainer::train(net::NeuralNet *network, const std::vector< std::vector<double> > &input, const std::vector< std::vector<double> > &correctOutput) {
  double totalError = 0;
  int iterations = 0;
  resetNetworkVectors(network);

  do {
    for(unsigned int a = 0; a < input.size(); a++) {
      trainOnDataPoint(network, input[a], correctOutput[a]);
    }

    totalError = 0;
    for(unsigned int trialIndex = 0; trialIndex < input.size(); trialIndex++) {
      std::vector<double> output = network->getOutput(input[trialIndex]);
      for(unsigned int outputIndex = 0; outputIndex < output.size(); outputIndex++) {
        totalError += pow(correctOutput[trialIndex][outputIndex] - output[outputIndex], 2);
      }
    }
    iterations++;
  } while(totalError > targetErrorLevel && iterations < maximumEpochs);

  //if(iterations >= maximumEpochs-1) std::cout << "SGDTrainer hit max epochs with an error level of " << totalError << ".\n";

  finalWeights = network->getWeights3D();

  return totalError;
}

double SGDTrainer::trainEpocs(double numberOfEpochs, net::NeuralNet *network, const std::vector< std::vector<double> > &input, const std::vector< std::vector<double> > &correctOutput) {
  resetNetworkVectors(network);

  double totalError;
  for(unsigned int epochs = 0; epochs < numberOfEpochs; epochs++) {
    totalError = 0;
    for(unsigned int a = 0; a < input.size(); a++) {
      totalError += trainOnDataPoint(network, input[a], correctOutput[a]);
    }
  }

  finalWeights = network->getWeights3D();

  return totalError;
}


double SGDTrainer::trainOnDataPoint(net::NeuralNet *network, const std::vector<double> &input, const std::vector<double> &correctOutput) {
  std::vector< std::vector< std::vector<double> > > weights = network->getWeights3D();
  gradients.push_back(network->getGradients(input, correctOutput));

  // Update weights
  std::vector< std::vector< std::vector<double> > > newWeightChanges(weights);
  for(unsigned int layerIndex = 0; layerIndex < weights.size(); layerIndex++) {
    for(unsigned int neuronIndex = 0; neuronIndex < weights[layerIndex].size(); neuronIndex++) {
      for(unsigned int weightIndex = 0; weightIndex < weights[layerIndex][neuronIndex].size(); weightIndex++) {
        double deltaWeight = getChangeInWeight(weights[layerIndex][neuronIndex][weightIndex], layerIndex, neuronIndex, weightIndex);
        weights[layerIndex][neuronIndex][weightIndex] += deltaWeight;
        newWeightChanges[layerIndex][neuronIndex][weightIndex] = deltaWeight;
      }
    }
  }
  weightChanges.push_back(newWeightChanges);

  network->setWeights3D(weights);

  double networkError = 0;
  std::vector<double> output = network->getOutput(input);
  for(unsigned int outputIndex = 0; outputIndex < output.size(); outputIndex++) {
    networkError += pow(correctOutput[outputIndex] - output[outputIndex], 2);
  }

  return networkError;
}


void SGDTrainer::resetNetworkVectors(net::NeuralNet *network) {
  initialWeights = network->getWeights3D();

  weightChanges.clear();

  finalWeights.clear();
  gradients.clear();
}


void SGDTrainer::store(std::ofstream *out) {
  assert(out->is_open());
  *out << targetErrorLevel << " " << maximumEpochs << "\n";
}

void SGDTrainer::initFromStream(std::ifstream *in) {
  assert(in->is_open());
  *in >> targetErrorLevel >> maximumEpochs;
}
