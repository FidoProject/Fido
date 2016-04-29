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
	resetVectors(network);

	do {
		totalError = 0;
		for(unsigned int a = 0; a < input.size(); a++) {
			totalError += trainOnDataPoint(network, input[a], correctOutput[a]);
		}
		iterations++;
 	} while(totalError > targetErrorLevel && iterations < maximumEpochs);

	if(iterations >= maximumEpochs-1) std::cout << "Adadelta hit max epochs with an error level of " << totalError << ".\n";

  finalWeights = network->getWeights3D();
}

double Adadelta::trainOnDataPoint(net::NeuralNet *network, const std::vector<double> &input, const std::vector<double> &correctOutput) {
	std::vector< std::vector< std::vector<double> > > weights = network->getWeights3D();
	gradients.push_back(network->getGradients(input, correctOutput));

  std::vector< std::vector< std::vector<double> > > newWeightChanges(weights);
  for(unsigned int layerIndex = 0; layerIndex < weights.size(); layerIndex++) {
		for(unsigned int neuronIndex = 0; neuronIndex < weights[layerIndex].size(); neuronIndex++) {
			for(unsigned int weightIndex = 0; weightIndex < weights[layerIndex][neuronIndex].size(); weightIndex++) {
        double grad = gradients.back()[layerIndex][neuronIndex][weightIndex];
				accumulatedGradients[layerIndex][neuronIndex][weightIndex] = rho*accumulatedGradients[layerIndex][neuronIndex][weightIndex] + (1.0-rho)*grad*grad;

        double dx = sqrt((accumulatedUpdates[layerIndex][neuronIndex][weightIndex]+epsilon)/(accumulatedGradients[layerIndex][neuronIndex][weightIndex]+epsilon)) * grad;
        accumulatedUpdates[layerIndex][neuronIndex][weightIndex] = rho*accumulatedUpdates[layerIndex][neuronIndex][weightIndex] + (1-rho)*dx*dx;

        newWeightChanges[layerIndex][neuronIndex][weightIndex] = dx;
        if(weightIndex == weights[layerIndex][neuronIndex].size()-1) {
          weights[layerIndex][neuronIndex][weightIndex] += dx;
        } else {
          weights[layerIndex][neuronIndex][weightIndex] += dx;
        }
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

void Adadelta::store(std::ofstream *output) {

}

void Adadelta::resetVectors(net::NeuralNet *network) {
  initialWeights = network->getWeights3D();

  accumulatedGradients = std::vector< std::vector< std::vector<double> > >(initialWeights);
  for(auto &a : accumulatedGradients) for(auto &b : a) std::fill(b.begin(), b.end(), 0);
  accumulatedUpdates = std::vector< std::vector< std::vector<double> > >(initialWeights);
  for(auto &a : accumulatedUpdates) for(auto &b : a) std::fill(b.begin(), b.end(), 0);

	weightChanges.clear();

	finalWeights.clear();
	gradients.clear();
}
