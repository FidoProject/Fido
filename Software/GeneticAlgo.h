#ifndef GENETICALGO_H
#define GENETICALGO_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <map>

#include "NeuralNet.h"


class GeneticAlgo {
public:
	std::map<double, NeuralNet > population;

	GeneticAlgo(int populationSize_, float mutationRate_, float crossoverRate_, double(*getFitness_)(NeuralNet neuralNet));
	void crossover(NeuralNet mom, NeuralNet dad, NeuralNet& offspring1, NeuralNet& offspring2);
	NeuralNet mutate(NeuralNet net);
	NeuralNet getBestNeuralNetwork(int numberOfGenerations);
	double(*getFitness)(NeuralNet neuralNet);


	int populationSize, numberOfGenerations;
	float mutationRate, crossoverRate;
};

#endif