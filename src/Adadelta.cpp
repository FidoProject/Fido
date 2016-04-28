#include "../include/Adadelta.h"

#include <climits>
#include <algorithm>

#include "../include/NeuralNet.h"

using namespace net;

Adadelta::Adadelta(double rho_, double targetErrorLevel_, int maximumEpochs_) {
  rho = rho_;
  targetErrorLevel = targetErrorLevel_;
  maximumEpochs = maximumEpochs_;
  epsilon = 0.000001;
}

void Adadelta::train(net::NeuralNet *network, const std::vector< std::vector<double> > &input, const std::vector< std::vector<double> > &correctOutput) {
  double totalError = 0;
	int iterations = 0;
	resetAccumulatedVectors(network);

	do {
		totalError = 0;
		for(unsigned int a = 0; a < input.size(); a++) {
			totalError += trainOnDataPoint(network, input[a], correctOutput[a]);
		}
		iterations++;
 	} while(totalError > targetErrorLevel && iterations < maximumEpochs);

	if(iterations >= maximumEpochs-1) std::cout << "Adadelta hit max epochs with an error level of " << totalError << ".\n";
}

double Adadelta::trainOnDataPoint(net::NeuralNet *network, const std::vector<double> &input, const std::vector<double> &correctOutput) {
	std::vector< std::vector< std::vector<double> > > weights = network->getWeights3D();
	std::vector< std::vector< std::vector<double> > > gradients = network->getGradients(input, correctOutput);

  for(unsigned int layerIndex = 0; layerIndex < weights.size(); layerIndex++) {
		for(unsigned int neuronIndex = 0; neuronIndex < weights[layerIndex].size(); neuronIndex++) {
      //std::cout << "Neuron: ";
			for(unsigned int weightIndex = 0; weightIndex < weights[layerIndex][neuronIndex].size(); weightIndex++) {
        double grad = gradients[layerIndex][neuronIndex][weightIndex];
				accumulatedGradients[layerIndex][neuronIndex][weightIndex] = rho*accumulatedGradients[layerIndex][neuronIndex][weightIndex] + (1.0-rho)*grad*grad;
        double dx = sqrt((accumulatedUpdates[layerIndex][neuronIndex][weightIndex]+epsilon)/(accumulatedGradients[layerIndex][neuronIndex][weightIndex]+epsilon)) * grad;
        accumulatedUpdates[layerIndex][neuronIndex][weightIndex] = rho*accumulatedUpdates[layerIndex][neuronIndex][weightIndex] + (1-rho)*dx*dx;
        if(weightIndex == weights[layerIndex][neuronIndex].size()-1) {
          weights[layerIndex][neuronIndex][weightIndex] += dx;
        } else {
          weights[layerIndex][neuronIndex][weightIndex] += dx;
        }
        //std::cout << weights[layerIndex][neuronIndex][weightIndex] << ", ";
			}
      //std::cout << "\n";
		}
	}

  network->setWeights3D(weights);

  double networkError = 0;
  std::vector<double> output = network->getOutput(input);
	for(unsigned int outputIndex = 0; outputIndex < output.size(); outputIndex++) {
		networkError += pow(correctOutput[outputIndex] - output[outputIndex], 2);
	}

	return networkError;
}


void Adadelta::prune(NeuralNet *network, const std::vector< std::vector< std::vector<double> > > &initialWeights, const std::vector< std::vector< std::vector< std::vector<double> > > > &weightChanges, unsigned int numNeuronsToPrune) {
  std::vector< std::vector< std::vector<double> > > finalWeights = network->getWeights3D();

  std::cout << "Num neurons " << network->numberOfHiddenNeurons() << "\n";
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
            sensitivity += fabs(pow(weightChange[a][b][c], 2) * finalWeights[a][b][c]/(1 * (finalWeights[a][b][c]-initialWeights[a][b][c])));
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

void Adadelta::store(std::ofstream *output) {

}

void Adadelta::resetAccumulatedVectors(net::NeuralNet *network) {
  accumulatedGradients.clear();
  accumulatedUpdates.clear();
  std::vector< std::vector< std::vector<double> > > weights = network->getWeights3D();
  for(unsigned int a = 0; a < weights.size(); a++) {
    std::vector< std::vector<double> > layer;
    for(unsigned int b = 0; b < weights[a].size(); b++) {
      std::vector<double> neuron;
      for(unsigned int c = 0; c < weights[a][b].size(); c++) {
        neuron.push_back(0);
      }
      layer.push_back(neuron);
    }
    accumulatedGradients.push_back(layer);
    accumulatedUpdates.push_back(layer);
  }
}
