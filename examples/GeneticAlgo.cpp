#include "Fido/GeneticAlgo.h"
#include <math.h>

std::vector<double> getPopulationFitness(const std::vector<net::NeuralNet> &population) {
  std::vector<double> fitnesses;

  for(auto const network : population) {
    double totalError = 0;
    for(unsigned double input = 1; input < 4; input++) {
      totalError += pow(input*2 - network.getOutput({input})[0], 2);
    }
    fitnesses.push_back(1/totalError);
  }

  return fitnesses;
}

int main() {
  srand(time(NULL));
  gen::GeneticAlgo geneticAlgo = gen::GeneticAlgo(20, 0.4, 0.5, 10, getPopulationFitness);
  net::NeuralNet modelNetwork = net::NeuralNet(1, 1, 1, 4, "simpleLinear");
  net::NeuralNet bestNetwork = geneticAlgo.getBestNeuralNetwork(600, modelNetwork);
}
