#include "GeneticAlgo.h"

GeneticAlgo::GeneticAlgo(int populationSize_, int numberOfGenerations_, int mutationRate_, int crossoverRate_) {
	populationSize = populationSize_;
	numberOfGenerations = numberOfGenerations_;
	mutationRate = mutationRate_;
	crossoverRate = crossoverRate_;
}

void GeneticAlgo::crossover(NeuralNet mom, NeuralNet dad, NeuralNet& offspring1, NeuralNet& offspring2) {

}

NeuralNet mutate(NeuralNet net) {

}

NeuralNet GeneticAlgo::getBest(int (*fitnessFunc)(NeuralNet net1)) {

}