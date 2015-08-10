#include "GeneticAlgo.h"
using namespace gen;

GeneticAlgo::GeneticAlgo(int populationSize_, float mutationRate_, float crossoverRate_, std::vector<double>(*getPopulationFitness_)(std::vector<net::NeuralNet> neuralNet)) {
	populationSize = populationSize_;
	mutationRate = mutationRate_;
	crossoverRate = crossoverRate_;
	getPopulationFitness = getPopulationFitness_;
}

void GeneticAlgo::crossover(net::NeuralNet &mom, net::NeuralNet &dad, net::NeuralNet &offspring1, net::NeuralNet &offspring2) {
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

	/// Crossover index must be a minimum of 1 and a maxiumum of the second to last index of the weights
	int crossoverIndex = (rand() % (momWeights.size() - 2)) + 1;

	for(int a = 0; a < crossoverIndex; a++) {
		offspring1Weights.push_back(momWeights[a]);
		offspring2Weights.push_back(dadWeights[a]);
	}
	for(int a = crossoverIndex; a < momWeights.size(); a++) {
		offspring1Weights.push_back(dadWeights[a]);
		offspring2Weights.push_back(momWeights[a]);
	}
    
	offspring1 = net::NeuralNet(mom);
	offspring1.setWeights(offspring1Weights);
	offspring2 = net::NeuralNet(dad);
	offspring2.setWeights(offspring2Weights);
    
    std::vector<double>().swap(offspring1Weights);
    std::vector<double>().swap(offspring2Weights);
    std::vector<double>().swap(momWeights);
    std::vector<double>().swap(dadWeights);
}

void GeneticAlgo::mutate(net::NeuralNet &net) {
	std::vector<double> weights = net.getWeights();

	for(int a = 0; a < weights.size(); a++) {
		float mutationDeterminer = rand() / RAND_MAX;
		if(mutationDeterminer <= mutationRate) {
			weights[a] = net::Neuron::randomWeight();
		}
	}

	net = net::NeuralNet(net);
	net.setWeights(weights);
    
    std::vector<double>().swap(weights);
}


net::NeuralNet GeneticAlgo::selectNNBasedOnFitness() {
	double totalFitnessOfPopulation = 0;
	for(int a = 0; a < fitnesses.size(); a++) totalFitnessOfPopulation += fitnesses[a];
	double cutOff = (double)((rand() / RAND_MAX) * totalFitnessOfPopulation);
	
	double fitnessCounter = 0;
	for (int a = 0; a < fitnesses.size(); a++) {
		fitnessCounter += fitnesses[a];
		if(fitnessCounter >= cutOff) {
			return population[a];
		}
	}
	std::cout << "There was an error in selecting a network based on fitness\n";
	throw 1;
}

net::NeuralNet GeneticAlgo::getBestNeuralNetwork(int numberOfGenerations, net::NeuralNet &modelNetwork) {
	population.clear();
	fitnesses.clear();

	std::vector<net::NeuralNet> bestNeuralNets;
	
	std::cout << "Starting generation\n";
	for(int a = 0; a < populationSize; a++) population.push_back(net::NeuralNet(modelNetwork));
	fitnesses = getPopulationFitness(population);

	for(int a = 0; a < numberOfGenerations; a++) {
        std::cout << "Generation: " << a << "\n";
        std::vector<net::NeuralNet> nextGeneration;
        
        /// Find the most fit neural network and add more to the population
        int mostFitIndex = 0;
        for(int a = 1; a < fitnesses.size(); a++) {
           if(fitnesses[a] > fitnesses[mostFitIndex]) mostFitIndex = a;
		}
		bestNeuralNets.push_back(population[mostFitIndex]);
		//std::cout << "number: " << (int)(population.size() / 10.0) << "\n";
        //for(int a = 0; a < (int)(population.size()/10.0); a++) nextGeneration.push_back(population[mostFitIndex]);
        
		while(nextGeneration.size() < populationSize) {
			net::NeuralNet parent1 = selectNNBasedOnFitness(), parent2 = selectNNBasedOnFitness();
			net::NeuralNet baby1, baby2;

			crossover(parent1, parent2, baby1, baby2);
			mutate(baby1);
			mutate(baby2);

			nextGeneration.push_back(baby1);
			nextGeneration.push_back(baby2);
		}
		std::vector<net::NeuralNet>().swap(population);
		population = nextGeneration;
        std::vector<net::NeuralNet>().swap(nextGeneration);
		fitnesses = getPopulationFitness(population);
	}

	std::vector<double> bestNeuralNetsFitnesses = getPopulationFitness(bestNeuralNets);
	std::cout << "bestNeuralNetsFitnesses: ";
	for(int a = 0; a < bestNeuralNetsFitnesses.size(); a++) std::cout << bestNeuralNetsFitnesses[a] << ", ";
	std::cout << "\n";

	int mostFitIndex = 0;
	for(int a = 1; a < fitnesses.size(); a++) {
		if(fitnesses[a] > fitnesses[mostFitIndex]) mostFitIndex = a;
	}
	return population[mostFitIndex];
}
