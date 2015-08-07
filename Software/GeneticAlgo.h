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
		std::map<double, net::NeuralNet > population;

		GeneticAlgo(int populationSize_, float mutationRate_, float crossoverRate_, double(*getFitness_)(net::NeuralNet neuralNet));
		void crossover(net::NeuralNet mom, net::NeuralNet dad, net::NeuralNet& offspring1, net::NeuralNet& offspring2);
		net::NeuralNet mutate(net::NeuralNet net);
		net::NeuralNet getBestNeuralNetwork(int numberOfGenerations);
		double(*getFitness)(net::NeuralNet neuralNet);


		int populationSize, numberOfGenerations;
		float mutationRate, crossoverRate;
	};
}

#endif