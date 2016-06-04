#include "../include/NeuralNet.h"

#include <iostream>

#include <assert.h>
#include <sstream>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>

using namespace net;

NeuralNet::NeuralNet() {
}

NeuralNet::NeuralNet(unsigned int numInputs, unsigned int numOutputs, unsigned int numHiddenLayers, unsigned int numNeuronsPerHiddenLayer, std::string activationFunctionName) {
  Layer firstHiddenLayer;
  for(unsigned int a = 0; a < numNeuronsPerHiddenLayer; a++) {
    firstHiddenLayer.neurons.push_back(Neuron(numInputs));
  }
  net.push_back(firstHiddenLayer);

  for(unsigned int a = 0; a < numHiddenLayers - 1; a++) {
    Layer hiddenLayer;
    for(unsigned int b = 0; b < numNeuronsPerHiddenLayer; b++) {
      hiddenLayer.neurons.push_back(Neuron(numNeuronsPerHiddenLayer));
    }
    net.push_back(hiddenLayer);
  }

  Layer outputLayer;
  for(unsigned int a = 0; a < numOutputs; a++) {
    outputLayer.neurons.push_back(Neuron(numNeuronsPerHiddenLayer));
  }
  net.push_back(outputLayer);

  for(unsigned int a = 0; a < net.size(); a++) {
    net[a].setActivationFunctionWithName(activationFunctionName);
  }

  randomizeWeights();
}

NeuralNet::NeuralNet(NeuralNet* otherNet) {
  net = std::vector< Layer >(otherNet->net);
  randomizeWeights();
}

NeuralNet::NeuralNet(std::ifstream *input) {
  assert(input->is_open());
  int numLayers;
  *input >> numLayers;
  for(unsigned int a = 0; a < numLayers; a++) {
    net.push_back(Layer(input));
  }
}

void NeuralNet::store(std::ofstream *output) {
  assert(output->is_open());
  // Output the net to the file
  *output << net.size() << "\n";
  for(unsigned int a = 0; a < net.size(); a++) {
    net[a].store(output);
  }
}

std::vector<double> NeuralNet::getWeights() {
  std::vector<double> weights;
  for(unsigned int a = 0; a < net.size(); a++) {
    for(unsigned int b = 0; b < net[a].neurons.size(); b++) {
      for(unsigned int c = 0; c < net[a].neurons[b].weights.size(); c++) {
        weights.push_back(net[a].neurons[b].weights[c]);
      }
    }
  }
  return weights;
}

std::vector< std::vector< std::vector<double> > > NeuralNet::getWeights3D() {
  std::vector< std::vector< std::vector<double> > > weights;
  for(unsigned int a = 0; a < net.size(); a++) {
    std::vector< std::vector<double> > layer;
    for(unsigned int b = 0; b < net[a].neurons.size(); b++) {
      layer.push_back(net[a].neurons[b].weights);
    }
    weights.push_back(layer);
  }
  return weights;
}

void NeuralNet::setWeights(std::vector<double> w) {
  unsigned int counter = 0;
  for(unsigned int a = 0; a < net.size(); a++) {
    for(unsigned int b = 0; b < net[a].neurons.size(); b++) {
      for(unsigned int c = 0; c < net[a].neurons[b].weights.size(); c++) {
        net[a].neurons[b].weights[c] = w[counter];
        counter++;
      }
    }
  }
  assert(counter == w.size());
}

void NeuralNet::setWeights3D(std::vector< std::vector< std::vector<double> > > w) {
  for(unsigned int a = 0; a < net.size(); a++) {
    for(unsigned int b = 0; b < net[a].neurons.size(); b++) {
      net[a].neurons[b].weights = w[a][b];
    }
  }
}

void NeuralNet::randomizeWeights() {
  for(unsigned int a = 0; a < net.size(); a++) for(unsigned int b = 0; b < net[a].neurons.size(); b++) {
    net[a].neurons[b].randomizeWeights();
  }
}

std::vector<double> NeuralNet::getOutput(std::vector<double> input) {
  std::vector<double> output;
  for(unsigned int a = 0; a < net.size(); a++) {
    if(a > 0) input = output;
    output.clear();
    for(unsigned int b = 0; b < net[a].neurons.size(); b++) {
      double out = net[a].activationFunction(net[a].neurons[b].getOutput(input));
      output.push_back(out);
    }
  }
  return output;
}

std::vector< std::vector<double> > NeuralNet::feedForward(std::vector<double> input) {
  std::vector< std::vector<double> > output;
  for(unsigned int a = 0; a < net.size(); a++) {
    if(a > 0) input = output[output.size() - 1];

    std::vector<double> outputLayer;

    for(unsigned int b = 0; b < net[a].neurons.size(); b++) {
      double out = net[a].activationFunction(net[a].neurons[b].getOutput(input));
      outputLayer.push_back(out);
    }

    output.push_back(outputLayer);
  }
  return output;
}

std::vector< std::vector< std::vector<double> > > NeuralNet::getGradients(const std::vector<double> &input, const std::vector<double> &correctOutput) {
  std::vector< std::vector<double> > outputs = feedForward(input);
  std::vector< std::vector< std::vector<double> > > weights = getWeights3D();
  std::vector< std::vector<double> > errors;
  ActivationFunction hiddenActivationFunctionDerivative = Layer::getDerivedActivationFunctionNames()[getHiddenActivationFunctionName()];
  ActivationFunction outputActivationFunctionDerivative = Layer::getDerivedActivationFunctionNames()[getOutputActivationFunctionName()];

  // Compute output layer error
  std::vector<double> outputNeuronErrors;
  std::vector<double> outputLayerOutput = outputs[outputs.size() - 1];
  for(unsigned int neuronIndex = 0; neuronIndex < outputLayerOutput.size(); neuronIndex++) {
    double outputNeuronError = (correctOutput[neuronIndex] - outputLayerOutput[neuronIndex]) * outputActivationFunctionDerivative(outputLayerOutput[neuronIndex]);
    outputNeuronErrors.push_back(outputNeuronError);
  }
  errors.push_back(outputNeuronErrors);

  // Compute hidden layer error
  for(int layerIndex = net.size() - 2; layerIndex >= 0; layerIndex--) {
    std::vector<double> currentLayerError;
    const std::vector<double> &currentHiddenLayerOutput = outputs[layerIndex];
    const std::vector<double> &lastLayerError = errors[errors.size() - 1];
    const std::vector< std::vector<double> > &lastLayerWeights = weights[layerIndex + 1];

    for(unsigned int neuronIndex = 0; neuronIndex < net[layerIndex].neurons.size(); neuronIndex++) {
      double errorsTimesWeights = 0;
      for(unsigned int previousNeuronIndex = 0; previousNeuronIndex < lastLayerError.size(); previousNeuronIndex++) {
        errorsTimesWeights += lastLayerError[previousNeuronIndex] * lastLayerWeights[previousNeuronIndex][neuronIndex];
      }
      double hiddenNeuronError = hiddenActivationFunctionDerivative(currentHiddenLayerOutput[neuronIndex]) * errorsTimesWeights;
      currentLayerError.push_back(hiddenNeuronError);
    }
    errors.push_back(currentLayerError);
  }

  // Compute gradients
  std::vector< std::vector< std::vector<double> > > gradients(weights.size());
  for(unsigned int errorIndex = 0; errorIndex < errors.size(); errorIndex++) {
    int layerIndex = ((int)errors.size() - 1) - errorIndex;
    std::vector< std::vector<double> > layerGradient;

    for(unsigned int neuronIndex = 0; neuronIndex < errors[errorIndex].size(); neuronIndex++) {
      std::vector<double> neuronGradient;

      if(layerIndex == 0) {
        for(unsigned int inputIndex = 0; inputIndex < input.size(); inputIndex++) {
          neuronGradient.push_back(errors[errorIndex][neuronIndex] * input[inputIndex]);
        }
      } else {
        for(unsigned int previousOutput = 0; previousOutput < outputs[layerIndex - 1].size(); previousOutput++) {
          neuronGradient.push_back(errors[errorIndex][neuronIndex] * outputs[layerIndex - 1][previousOutput]);
        }
      }

      // Activation weight gradient
      neuronGradient.push_back(-errors[errorIndex][neuronIndex]);

      layerGradient.push_back(neuronGradient);
    }

    gradients[layerIndex] = layerGradient;
  }

  return gradients;
}

void NeuralNet::printWeights() {
  std::cout << "Neuron weights: \n";
  for(unsigned int a = 0; a < net.size(); a++) {
    std::cout << "  Layer " << a << ":\n";
    for(unsigned int b = 0; b < net[a].neurons.size(); b++) {
      std::cout << "    Neuron " << b << ": ";
      for(unsigned int c = 0; c < net[a].neurons[b].weights.size(); c++) {
        std::cout << net[a].neurons[b].weights[c] << " ";
      }
      std::cout << "\n";
    }
  }
}

unsigned int NeuralNet::numberOfHiddenLayers() {
  return net.size() - 1;
}

unsigned int  NeuralNet::numberOfInputs() {
  return net[0].neurons[0].weights.size() - 1;
}

unsigned int NeuralNet::numberOfOutputs() {
  return net[net.size() - 1].neurons.size();
}

unsigned int NeuralNet::numberOfHiddenNeurons() {
  unsigned int num = 0;
  for(unsigned int a = 0; a < net.size()-1; a++) {
    for(unsigned int b = 0; b < net[a].neurons.size(); b++) {
      num++;
    }
  }

  return num;
}

void NeuralNet::setOutputActivationFunction(std::string name) {
  net[net.size() - 1].setActivationFunctionWithName(name);
}

std::string NeuralNet::getHiddenActivationFunctionName() {
  return net[0].getActivationFunctionName();
}

std::string NeuralNet::getOutputActivationFunctionName() {
  return net[net.size() - 1].getActivationFunctionName();
}

void NeuralNet::removeNeuron(unsigned int hiddenLayerIndex, unsigned int neuronIndex) {
  assert(hiddenLayerIndex < numberOfHiddenLayers());
  assert(neuronIndex < net[hiddenLayerIndex].neurons.size());
  net[hiddenLayerIndex].neurons.erase(net[hiddenLayerIndex].neurons.begin() + neuronIndex);
  for(unsigned int nextLayerNeuronIndex = 0; nextLayerNeuronIndex < net[hiddenLayerIndex+1].neurons.size(); nextLayerNeuronIndex++) {
    std::vector<double> &nextLayerWeightsRef = net[hiddenLayerIndex+1].neurons[nextLayerNeuronIndex].weights;
    nextLayerWeightsRef.erase(nextLayerWeightsRef.begin() + neuronIndex);
  }
}
