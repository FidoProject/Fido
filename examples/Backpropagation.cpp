#include "Fido/Fido.h"

int main() {
  net::NeuralNet neuralNetwork = net::NeuralNet(1, 1, 2, 4, "sigmoid");
  neuralNetwork.setOutputActivationFunction("simpleLinear");

  std::vector< std::vector<double> > input = { {1}, {2}, {5}, {6} };
  std::vector< std::vector<double> > correctOutput = { {2}, {4}, {10}, {12} };

  net::Backpropagation backprop = net::Backpropagation();
  backprop.train(&neuralNetwork, input, correctOutput);
}
