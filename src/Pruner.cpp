#include "../include/Pruner.h"

#include <iostream>
#include <algorithm>
#include <math.h>
#include <climits>

#include "../include/NeuralNet.h"
#include "../include/Backpropagation.h"

using namespace net;

Pruner::Pruner() { }

void Pruner::pruneRandomnly(NeuralNet *network) {
  int hiddenLayerIndex = rand() % network->numberOfHiddenLayers();
  network->removeNeuron(hiddenLayerIndex, rand() % network->net[hiddenLayerIndex].neurons.size());
}

void Pruner::prune(NeuralNet *network, Trainer *networkTrainer) {
  prune(network, networkTrainer->getInitialWeights(), networkTrainer->getWeightChanges(), networkTrainer->getGradients());
}

void Pruner::prune(NeuralNet *network, const std::vector< std::vector< std::vector<double> > > &initialWeights, const std::vector< std::vector< std::vector< std::vector<double> > > > &weightChanges, const std::vector< std::vector< std::vector< std::vector<double> > > > &gradients) {

  std::vector< std::vector< std::vector<double> > > finalWeights = network->getWeights3D();

  // Get sensitivities of all neurons
  std::vector<std::vector<double> > sensitivities;
  for(unsigned int layerIndex = 0; layerIndex < network->numberOfHiddenLayers(); layerIndex++) {
    std::vector<double> layerSensitivities;
    for(unsigned int neuronIndex = 0; neuronIndex < network->net[layerIndex].neurons.size(); neuronIndex++) {
      double neuronSensitivity = 0;
      for(unsigned int weightIndex = 0; weightIndex < network->net[layerIndex].neurons[neuronIndex].weights.size(); weightIndex++) {
        if(finalWeights[layerIndex][neuronIndex][weightIndex]-initialWeights[layerIndex][neuronIndex][weightIndex] < 0.001) {
          for(unsigned int trainingEpoch = 0; trainingEpoch < weightChanges.size(); trainingEpoch++) {
            neuronSensitivity += fabs(fabs(gradients[trainingEpoch][layerIndex][neuronIndex][weightIndex]) * fabs(weightChanges[trainingEpoch][layerIndex][neuronIndex][weightIndex]) * finalWeights[layerIndex][neuronIndex][weightIndex]/(finalWeights[layerIndex][neuronIndex][weightIndex]-initialWeights[layerIndex][neuronIndex][weightIndex]));
          }
        } else {
          neuronSensitivity += 100;
        }
      }

      neuronSensitivity /= weightChanges.size()*network->net[layerIndex].neurons[neuronIndex].weights.size();
      layerSensitivities.push_back(neuronSensitivity);
    }
    sensitivities.push_back(layerSensitivities);
  }

  // Prune neuron
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

}
