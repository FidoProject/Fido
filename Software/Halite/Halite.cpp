#include "Halite.h"


HalitePlayer Halite::player = HalitePlayer();
net::NeuralNet* Halite::getBestPlayer(int numberOfIterations) {
	gen::GeneticAlgo geneticAlgo(40, 0.2, 0.6, 5, getPopulationFitnesses);

	player.connect();

	net::NeuralNet modelNet(18, 1, 5, 20, net::NeuralNet::sigmoid);

	return geneticAlgo.getBestNeuralNetwork(numberOfIterations, modelNet);
}

std::vector<double> Halite::getPopulationFitnesses(std::vector<net::NeuralNet *> players) {
	std::vector<double> fitnesses;

	for(int a = 0; a < players.size(); a++) {
		double score = getScore(players[a]);
		std::cout << "Score: " << score << "\n";
		fitnesses.push_back(score);
	}

	for(int a = 0; a < fitnesses.size(); a++) fitnesses[a] = pow(fitnesses[a], 3);
	return fitnesses;
}

int Halite::getScore(net::NeuralNet *network) {
	player.network = network;
	return player.playGames();
}