#ifndef GENETICALGO_H
#define GENETICALGO_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <map>

#include "NeuralNet.h"

namespace gen {
	class GeneticAlgo {
	public:
		std::vector<net::NeuralNet> population;
		int populationSize, numberOfGenerations;
		float mutationRate, crossoverRate;

		GeneticAlgo(int populationSize_, float mutationRate_, float crossoverRate_, std::vector<double>(*getPopulationFitness_)(std::vector<net::NeuralNet> neuralNet));
		void crossover(net::NeuralNet mom, net::NeuralNet dad, net::NeuralNet& offspring1, net::NeuralNet& offspring2);
		net::NeuralNet mutate(net::NeuralNet net);
		net::NeuralNet getBestNeuralNetwork(int numberOfGenerations, net::NeuralNet modelNetwork);
		std::vector<double>(*getPopulationFitness)(std::vector<net::NeuralNet> neuralNet);
	};
}

#endif