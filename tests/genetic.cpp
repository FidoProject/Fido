#include <iostream>
#include "catch.h"

#include "../include/GeneticAlgo.h"

const static std::vector< std::vector<double> > input = { {1}, {2}, {3} };
const static std::vector< std::vector<double> > correctOutput = { {2}, {4}, {6} };
const static double acceptableError = 0.05;

std::vector<double> getPopulationFitness(const std::vector<net::NeuralNet> &population) {
  std::vector<double> fitnesses;

  for(net::NeuralNet network : population) {
    double totalError = 0;
    for(unsigned int inputIndex = 0; inputIndex < input.size(); inputIndex++) {
      totalError += pow(correctOutput[inputIndex][0] - network.getOutput(input[inputIndex])[0], 2);
    }
		fitnesses.push_back(1/totalError);
  }

  return fitnesses;
}

TEST_CASE("Test genetic algorithms on linear regression", "[genetic]") {
  srand(time(NULL));
  gen::GeneticAlgo geneticAlgo = gen::GeneticAlgo(20, 0.4, 0.5, 10, getPopulationFitness);
	net::NeuralNet modelNetwork = net::NeuralNet(1, 1, 1, 4, "simpleLinear");
  net::NeuralNet bestNetwork = geneticAlgo.getBestNeuralNetwork(600, modelNetwork);

  double totalError = 0;
  for(unsigned int inputIndex = 0; inputIndex < input.size(); inputIndex++) {
    totalError += fabs(correctOutput[inputIndex][0] - bestNetwork.getOutput(input[inputIndex])[0]);
  }

  REQUIRE(totalError / double(input.size()) < acceptableError);
}
