#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>

#include "Neuron.h"
#include "NeuralNet.h"
#include "GeneticAlgo.h"
#include "TicTacToe.h"
#include "Halite/Halite.h"
#include "Backpropagation.h"

int main() {
    srand(time(NULL));

	net::NeuralNet *player = new net::NeuralNet("C:/Users/Michael Truell/Documents/Fido/Software/PastNN/firsthalite.txt", net::NeuralNet::sigmoid);
	HalitePlayer hPlayer(player);
	hPlayer.connect();
	hPlayer.playGames();

	/*TicTacToe game;
	net::NeuralNet *bestPlayer = game.getBestPlayer(5000);
	bestPlayer->storeNet("C:/Users/Michael Truell/Documents/Fido/Software/PastNN/TTT_4by9_5000_1000.txt");
	std::cout << "Outcome: " << TicTacToe::getScoreAgainstRandomPlayers(bestPlayer, 10000) << "\n";
	TicTacToe::playAgainstHuman(bestPlayer);*/
} 