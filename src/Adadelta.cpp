#include "../include/Adadelta.h"

#include <climits>
#include <cassert>
#include <algorithm>

#include "../include/NeuralNet.h"

using namespace net;

Adadelta::Adadelta(double rho_, double targetErrorLevel_, int maximumEpochs_) : SGDTrainer(targetErrorLevel_, maximumEpochs_) {
  rho = rho_;
  epsilon = 0.000001;
}

Adadelta::Adadelta(std::ifstream *in) {
	initFromStream(in);
}

double Adadelta::getChangeInWeight(double weight, int layerIndex, int neuronIndex, int weightIndex) {
  double grad = gradients.back()[layerIndex][neuronIndex][weightIndex];
  accumulatedGradients[layerIndex][neuronIndex][weightIndex] = rho*accumulatedGradients[layerIndex][neuronIndex][weightIndex] + (1.0-rho)*grad*grad;

  double dx = sqrt((accumulatedUpdates[layerIndex][neuronIndex][weightIndex]+epsilon)/(accumulatedGradients[layerIndex][neuronIndex][weightIndex]+epsilon)) * grad;
  accumulatedUpdates[layerIndex][neuronIndex][weightIndex] = rho*accumulatedUpdates[layerIndex][neuronIndex][weightIndex] + (1-rho)*dx*dx;

  return dx;
}

void Adadelta::store(std::ofstream *output) {
  if(output->is_open()) {
		SGDTrainer::store(output);
		*output << rho << " " << epsilon << "\n";
	} else {
		std::cout << "Could not store backprop\n";
		throw 1;
	}
}

bool Adadelta::initFromStream(std::ifstream *in) {
  if(in->is_open()) {
		assert(SGDTrainer::initFromStream(in) == true);
		*in >> rho >> epsilon;
		return true;
	} else {
		std::cout << "Could not retrieve sgdtrainer from file\n";
		throw 1;
	}
}

void Adadelta::store(std::ofstream *output) {
	*output << "Adadelta " << targetErrorLevel << " " << maximumEpochs << " " << rho << " " << epsilon;
}

bool Adadelta::initFromStream(std::ifstream *in) {
	if(in->is_open()) {
		std::string name;
		*in >> name;
		if (name != "Adadelta") {
			return false;
		}
		*in >> targetErrorLevel >> maximumEpochs >> rho >> epsilon;
		return true;
	} else {
		std::cout << "Could not retrieve backprop from file\n";
		throw 1;
	}
}
void Adadelta::resetNetworkVectors(net::NeuralNet *network) {
  SGDTrainer::resetNetworkVectors(network);

  accumulatedGradients = std::vector< std::vector< std::vector<double> > >(initialWeights);
  for(auto &a : accumulatedGradients) for(auto &b : a) std::fill(b.begin(), b.end(), 0);
  accumulatedUpdates = std::vector< std::vector< std::vector<double> > >(initialWeights);
  for(auto &a : accumulatedUpdates) for(auto &b : a) std::fill(b.begin(), b.end(), 0);
}
