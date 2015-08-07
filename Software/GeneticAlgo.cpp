#include "GeneticAlgo.h"

GeneticAlgo::GeneticAlgo(int populationSize_, float mutationRate_, float crossoverRate_, double(*getFitness_)(NeuralNet neuralNet)) {
	populationSize = populationSize_;
	mutationRate = mutationRate_;
	crossoverRate = crossoverRate_;
	getFitness = getFitness_;
}

void GeneticAlgo::crossover(NeuralNet mom, NeuralNet dad, NeuralNet& offspring1, NeuralNet& offspring2) {
	float crossoverDeterminer = rand() / RAND_MAX;
	if(crossoverDeterminer > crossoverRate) {
		offspring1 = mom;
		offspring2 = dad;
		return;
	}
	std::vector<double> offspring1Weights;
	std::vector<double> offspring2Weights;
	std::vector<double> momWeights = mom.getWeights();
	std::vector<double> dadWeights = dad.getWeights();

	// Crossover index must be a minimum of 1 and a maxiumum of the second to last index of the weights
	int crossoverIndex = (rand() % momWeights.size() - 2) + 1;

	for(int a = 0; a < crossoverIndex; a++) {
		offspring1Weights.push_back(momWeights[a]);
		offspring2Weights.push_back(dadWeights[a]);
	}
	for(int a = crossoverIndex; a < momWeights.size(); a++) {
		offspring1Weights.push_back(dadWeights[a]);
		offspring2Weights.push_back(momWeights[a]);
	}
	offspring1 = NeuralNet(mom.numInputs, mom.numOutputs, mom.numHiddenLayers, mom.numNeruronsPerHiddenLayer);
	offspring1.setWeights(offspring1Weights);
	offspring2 = NeuralNet(mom.numInputs, mom.numOutputs, mom.numHiddenLayers, mom.numNeruronsPerHiddenLayer);
	offspring2.setWeights(offspring2Weights);
}
