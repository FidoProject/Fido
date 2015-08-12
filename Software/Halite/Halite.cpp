#include "Halite.h"

net::NeuralNet Halite::getBestPlayer(int numberOfIterations) {
	gen::GeneticAlgo geneticAlgo(40, 0.2, 0.6, 5, getPopulationFitnesses);

	net::NeuralNet modelNet(9, 1, 5, 9, net::NeuralNet::sigmoid);

	return geneticAlgo.getBestNeuralNetwork(numberOfIterations, modelNet);
}