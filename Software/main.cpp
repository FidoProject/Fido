#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>

#include "Neuron.h"
#include "NeuralNet.h"
#include "GeneticAlgo.h"
#include "TicTacToe.h"
#include "Backpropagation.h"

int main() {
    srand(time(NULL));
	/*TicTacToe game;

	net::NeuralNet *bestPlayer = game.getBestPlayer(1000);
	bestPlayer->storeNet("C:/Users/Michael Truell/Documents/Fido/Software/PastNN/TTT_2by9_1000_200.txt");

	clock_t begin = clock();
	std::cout << "Outcome: " << TicTacToe::getScoreAgainstRandomPlayers(bestPlayer, 1000) << "\n";
	clock_t end = clock();
	double elapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << "Time: " << elapsedSecs << "\n";

	TicTacToe::playAgainstHuman(bestPlayer);*/

	net::NeuralNet *network = new net::NeuralNet(1, 1, 2, 2, net::NeuralNet::sigmoid);
	Backpropogation prop = Backpropogation(0.5, 0.9, 0.001, 100000);
	std::vector< std::vector<double> > input = { { 0.05 }, { 0.1 }, {0.12}, { 0.3 }, { 0.33 }, { 0.4 }, { 0.45 }, };
	std::vector< std::vector<double> > correctOutput = { { 0.2 }, { 0.3 }, {0.34}, { 0.7 }, { 0.76 }, { 0.9 }, { 1.0 } };
	prop.trainOnData(network, input, correctOutput);

	for(int a = 0; a < 10; a++) {
		std::cout << "in: " << a / 20.0 << "; out: " << network->getOutput({ a / 20.0 })[0] << "\n";
	}

	network->storeNet("C:/Users/Michael Truell/Documents/Fido/Software/PastNN/VerySimpleRegression.txt");
} 