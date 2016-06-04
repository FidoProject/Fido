#include "../include/Neuron.h"

#include <assert.h>
#include <iostream>
#include <stdlib.h>

using namespace net;

Neuron::Neuron(int numInputs) {
  weights = std::vector<double>(numInputs+1);
  for(int a = 0; a < numInputs+1; a++) {
    weights[a] = Neuron::randomWeight();
  }
}

Neuron::Neuron(std::vector<double> w) {
  weights = w;
}

void Neuron::randomizeWeights() {
  for(unsigned int a = 0; a < weights.size(); a++) {
    weights[a] = Neuron::randomWeight();
  }
}

double Neuron::getOutput(std::vector<double> inputs) {
  assert(inputs.size() == weights.size() - 1);
  double returnNumber = 0;
  for(unsigned int a = 0; a < inputs.size(); a++) {
    returnNumber += inputs[a]*weights[a];
  }
  returnNumber += -1 * weights[weights.size()-1];
  return returnNumber;
}
