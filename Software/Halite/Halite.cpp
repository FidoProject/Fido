#include "Halite.h"


HalitePlayer Halite::player = HalitePlayer();
net::NeuralNet* Halite::getBestPlayer(int numberOfIterations) {
	gen::GeneticAlgo geneticAlgo(10, 0.2, 0.6, 2, getPopulationFitnesses);

	player.connect();

	net::NeuralNet modelNet(18, 1, 4, 18, net::NeuralNet::sigmoid);

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

	int mostFitIndex = 0;
	for(int a = 0; a < fitnesses.size(); a++) if(fitnesses[a] > fitnesses[mostFitIndex]) mostFitIndex = a;
	players[mostFitIndex]->storeNet("C:/Users/Michael Truell/Documents/Fido/Software/PastNN/firsthalite.txt");

	return fitnesses;
}

int Halite::getScore(net::NeuralNet *network) {
	player.network = network;
	return player.playGames();
}