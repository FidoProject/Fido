#include "Halite.h"


HalitePlayer Halite::player = HalitePlayer();
net::NeuralNet* Halite::getBestPlayer(int numberOfIterations) {
	gen::GeneticAlgo geneticAlgo(20, 0.25, 0.7, 2, getPopulationFitnesses);

	player.connect();

	net::NeuralNet *modelNet = new net::NeuralNet("C:/Users/Michael Truell/Documents/Fido/Software/PastNN/bestbasic.txt", net::NeuralNet::sigmoid);

	return geneticAlgo.getBestNeuralNetwork(numberOfIterations, modelNet);
}

std::vector<double> Halite::getPopulationFitnesses(std::vector<net::NeuralNet *> players) {
	std::vector<double> fitnesses;
	std::vector<double> times;

	bool allZero = true;
	for(int a = 0; a < players.size(); a++) {
		clock_t begin = clock();
		double score = getScore(players[a]);
		clock_t end = clock();
		double elapsed = double(end - begin) / (CLOCKS_PER_SEC);

		if(score != 0) allZero = false;

		double fitness = score + net::NeuralNet::sigmoid(-elapsed*0.2);

		std::cout << "Fitness: " << fitness << "\n";
		std::cout << "Time: " << elapsed << "\n";

		times.push_back(elapsed);
		fitnesses.push_back(fitness);
	}

	if(allZero) fitnesses = times;

	for(int a = 0; a < fitnesses.size(); a++) fitnesses[a] = pow(fitnesses[a], 2);

	int mostFitIndex = 0;
	for(int a = 0; a < fitnesses.size(); a++) if(fitnesses[a] > fitnesses[mostFitIndex]) mostFitIndex = a;
	players[mostFitIndex]->storeNet("C:/Users/Michael Truell/Documents/Fido/Software/PastNN/bestbasic.txt");

	return fitnesses;
}

int Halite::getScore(net::NeuralNet *network) {
	player.network = network;
	return player.playGames();
}