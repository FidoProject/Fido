#include "../include/GeneticAlgo.h"

#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <ctime>

using namespace gen;

GeneticAlgo::GeneticAlgo(unsigned int populationSize_, float mutationRate_, float crossoverRate_, int numberOfElitismCopies_, std::vector<double>(*getPopulationFitness_)(const std::vector<net::NeuralNet> &population)) {
  populationSize = populationSize_;
  mutationRate = mutationRate_;
  crossoverRate = crossoverRate_;
  userProvidedGetPopulationFitness = getPopulationFitness_;
  numberOfElitismCopies = numberOfElitismCopies_;
}

void GeneticAlgo::crossover(net::NeuralNet mom, net::NeuralNet dad, net::NeuralNet *offspring1, net::NeuralNet *offspring2) {
  float crossoverDeterminer = (float)rand() / (float)RAND_MAX;
  if(crossoverDeterminer > crossoverRate) {
    *offspring1 = mom;
    *offspring2 = dad;
    return;
  }

  std::vector<double> offspring1Weights;
  std::vector<double> offspring2Weights;
  std::vector<double> momWeights = mom.getWeights();
  std::vector<double> dadWeights = dad.getWeights();

  /// Crossover index must be a minimum of 1 and a maxiumum of the second to last index of the weights
  int crossoverIndex = (rand() % (momWeights.size() - 2)) + 1;

  for(unsigned int a = 0; a < crossoverIndex; a++) {
    offspring1Weights.push_back(momWeights[a]);
    offspring2Weights.push_back(dadWeights[a]);
  }
  for(unsigned int a = crossoverIndex; a < momWeights.size(); a++) {
    offspring1Weights.push_back(dadWeights[a]);
    offspring2Weights.push_back(momWeights[a]);
  }

  *offspring1 = net::NeuralNet(mom);
  offspring1->setWeights(offspring1Weights);
  *offspring2 = net::NeuralNet(dad);
  offspring2->setWeights(offspring2Weights);

  std::vector<double>().swap(offspring1Weights);
  std::vector<double>().swap(offspring2Weights);
  std::vector<double>().swap(momWeights);
  std::vector<double>().swap(dadWeights);
}

void GeneticAlgo::mutate(net::NeuralNet *net) {
  std::vector<double> weights = net->getWeights();

  for(unsigned int a = 0; a < weights.size(); a++) {
    float mutationDeterminer = (float)rand() / (float)RAND_MAX;
    if(mutationDeterminer <= mutationRate) {
      weights[a] = net::Neuron::randomWeight();
    }
  }

  net->setWeights(weights);

  std::vector<double>().swap(weights);
}


net::NeuralNet GeneticAlgo::selectNNBasedOnFitness() {
  double totalFitnessOfPopulation = 0;
  for(unsigned int a = 0; a < fitnesses.size(); a++) totalFitnessOfPopulation += fitnesses[a];
  double cutOff = (((double)rand() / (double)RAND_MAX) * totalFitnessOfPopulation);

  double fitnessCounter = 0;
  for(unsigned int a = 0; a < fitnesses.size(); a++) {
    fitnessCounter += fitnesses[a];
    if(fitnessCounter >= cutOff) {
      return population[a];
    }
  }
  assert(false);
}

void GeneticAlgo::createNextGeneration() {
  std::vector<net::NeuralNet> nextGeneration;

  int mostFitIndex = 0;
  for(unsigned int a = 1; a < fitnesses.size(); a++) {
    if(fitnesses[a] > fitnesses[mostFitIndex]) mostFitIndex = a;
  }

  for(unsigned int a = 0; a < numberOfElitismCopies; a++) {
    nextGeneration.push_back(population[mostFitIndex]);
  }

  while(nextGeneration.size() < populationSize) {
    net::NeuralNet parent1 = selectNNBasedOnFitness(), parent2 = selectNNBasedOnFitness();
    net::NeuralNet baby1, baby2;

    crossover(parent1, parent2, &baby1, &baby2);
    mutate(&baby1);
    mutate(&baby2);

    nextGeneration.push_back(baby1);
    nextGeneration.push_back(baby2);
  }

  population = nextGeneration;
  nextGeneration.clear();
}

net::NeuralNet GeneticAlgo::getBestNeuralNetwork(int numberOfGenerations, net::NeuralNet modelNetwork) {
  population.clear();
  fitnesses.clear();

  population.push_back(modelNetwork);
  for(unsigned int a = 0; a < populationSize-1; a++) {
    population.push_back(net::NeuralNet(modelNetwork));
  }

  for(unsigned int a = 0; a < numberOfGenerations; a++) {
    getPopulationFitness();
    createNextGeneration();
  }

  getPopulationFitness();

  int mostFitIndex = 0;
  for(unsigned int a = 1; a < fitnesses.size(); a++) {
    if(fitnesses[a] > fitnesses[mostFitIndex]) mostFitIndex = a;
  }

  return population[mostFitIndex];
}

void GeneticAlgo::getPopulationFitness() {
  fitnesses = userProvidedGetPopulationFitness(population);
  assert(fitnesses.size() == population.size());
}
