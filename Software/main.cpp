#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Neuron.h"
#include "NeuralNet.h"
#include "GeneticAlgo.h"
#include "TicTacToe.h"

/*
 Fix hasEnded
 Allow neural networks to be dumped and restored to/from files
 Add in safe-guards to functions (e.g net::getInput() needs to check that the input is of the right length)
 */

int main() {
    srand(time(NULL));
	std::cout << "Start\n";
	TicTacToe game;
	std::cout << "Finished intialization\n";

	net::NeuralNet bestPlayer("nn200.txt", net::NeuralNet::sigmoid);
	net::NeuralNet secondBestPlayer(9, 1, 3, 9, net::NeuralNet::sigmoid);
	std::cout << "W: ";
	std::vector<double> weights = bestPlayer.getWeights();
	for(int a = 0; a < weights.size(); a++) std::cout << weights[a] << ", ";

	std::cout << "\n\nW: ";
	weights = secondBestPlayer.getWeights();
	for(int a = 0; a < weights.size(); a++) std::cout << weights[a] << ", ";
    //bestPlayer.storeNet("nn200.txt");
	std::cout << "d: " << &bestPlayer;
	std::cout << "d: " << &secondBestPlayer;
	TicTacToe::playVisualGame(&bestPlayer, &secondBestPlayer);
	TicTacToe::playVisualGame(&secondBestPlayer, &bestPlayer);
	TicTacToe::playVisualGame(&bestPlayer, &bestPlayer);

	std::cout << "Outcome: " << TicTacToe::getWinsAgainstRandomPlayers(&secondBestPlayer, 100) << "\n";
	std::cout << "Outcome: " << TicTacToe::getWinsAgainstRandomPlayers(&bestPlayer, 100) << "\n";

    //net::NeuralNet secondBestPlayer = game.getBestPlayer(10);
    //secondBestPlayer.storeNet("Documents/Fido/Software/nn10.txt");

    //int outcome = TicTacToe::getOutcomeOfGame(&bestPlayer, &secondBestPlayer);
    //std::cout << "Outcome: " << outcome << "\n";

    std::cout << "DONE\n";
}