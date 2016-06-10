#pragma once

#include <vector>

#include "NeuralNet.h"

namespace gen {
/** Genetic algorithms used to produce fit neural networks. */
class GeneticAlgo {
public:
  /** Initializes a genetic algorithm with the necessary parameters.
   *
   * \param populationSize_ the number of neural networks in the population for each generation
   * \param mutationRate_ the chance between 0 and 1 that a weight in a neural network will be mutated
   * \param crossoverRate_ the chance between 0 and 1 that a couple will produce offspring having a fused verion of their parents' networks, instead of offspring that have the same neural networks as their parents; if set to 0, the population's composition will only change as a result of mutations.
   * \param numberOfElitismCopies_ the number of times that best neural network from the past generation shows up in the current generation
   * \param getPopulationFitness_ a function that returns a vector of doubles housing the fitness of every individual in the population, when given the population of neural networks
   */
  GeneticAlgo(unsigned int populationSize_, float mutationRate_, float crossoverRate_, int numberOfElitismCopies_, std::vector<double>(*getPopulationFitness_)(const std::vector<net::NeuralNet> &neuralNet));

  /** Returns the most fit neural network in a population that undergoes a specified number of generations.
   *
   * \param numberOfGenerations the number of generations that the simulation will run for; in each generation, fitnesses are assigned, mating occurs, and offspring are added
   * \param modelNetwork used to determine the architecuture of the neural networks in the population,.
   */
  net::NeuralNet getBestNeuralNetwork(int numberOfGenerations, net::NeuralNet modelNetwork);

private:
  std::vector<net::NeuralNet> population;
  std::vector<double> fitnesses;
  unsigned int populationSize, numberOfGenerations;
  float mutationRate, crossoverRate;

  /** Returns the fitness of every individual in the population, when given the population of neural networks. */
  std::vector<double>(*userProvidedGetPopulationFitness)(const std::vector<net::NeuralNet> &neuralNet);

  /*The number of times the most fit neural network of a generation is inserted directly into the subsequent generation.
   *
   * The practice of taking the most fit neural network of a generation and inserting it directly into the subsequent generation a number of times without mutation is referred to as "Elitism" for our purposes.
   */
  int numberOfElitismCopies;

  /* Produce two new neural networks from two parent neural networks.
   *
   * The method uses the crossoverRate variable to determine if two neural networks are mixed
   * and then and mixes their neurons weights to produce two new neural networks.
   */
  void crossover(net::NeuralNet mom, net::NeuralNet dad, net::NeuralNet *offspring1, net::NeuralNet *offspring2);

  // Randomnly change the weights of the inputs of a neural network based on the mutation rate.
  void mutate(net::NeuralNet *net);

  // Select a random network from the population with a weight towards higher fitness levels
  net::NeuralNet selectNNBasedOnFitness();

  // Uses the fitness and population values to mate fit individuals, mutate their weights, and place them into the next generation
  void createNextGeneration();

  void getPopulationFitness();
};
}
