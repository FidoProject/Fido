#ifndef GENETICALGO_H
#define GENETICALGO_H

#include <iostream>
#include <vector>
#include <map>

#include "NeuralNet.h"


class GeneticAlgo {
public:

	GeneticAlgo(int populationSize_, int numberOfGenerations_, int mutationRate_, int crossoverRate_);

	std::map<double, NeuralNet > population;

	void crossover(NeuralNet mom, NeuralNet dad, NeuralNet& offspring1, NeuralNet& offspring2);
	NeuralNet mutate(NeuralNet net);
	NeuralNet getBest(int (*fitnessFunc)(NeuralNet net1));

private:
	int populationSize, numberOfGenerations, mutationRate, crossoverRate;
};

#endif