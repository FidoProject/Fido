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
#include "QLearn.h"


int main() {
    srand(time(NULL));

	//net::NeuralNet *player = Halite::getBestPlayer(200);
	//player->storeNet("C:/Users/Michael Truell/Documents/Fido/Software/PastNN/bestbasic.txt");
	
	net::QLearn learn = net::QLearn(new net::NeuralNet(3, 1, 3, 3, net::NeuralNet::sigmoid), net::Backpropagation(0.5, 0.9, 0.003, 100000), 1, 0.5, 2);
	std::vector<double> state(1);
	double reward;
	for(int a = 0; a < 100; a++) {
		state[0] = 1;
		int action = learn.getAction(state);
		//if(action == 0) learn.applyReinforcement(0, );
	}

	/*net::NeuralNet *player = new net::NeuralNet("C:/Users/Michael Truell/Documents/Fido/Software/PastNN/bestbasic.txt", net::NeuralNet::sigmoid);
	HalitePlayer hPlayer = HalitePlayer(player);
	hPlayer.connect();
	hPlayer.playGames();*/

	/*TicTacToe game;
	net::NeuralNet *bestPlayer = game.getBestPlayer(5000);
	bestPlayer->storeNet("C:/Users/Michael Truell/Documents/Fido/Software/PastNN/TTT_4by9_5000_1000.txt");
	std::cout << "Outcome: " << TicTacToe::getScoreAgainstRandomPlayers(bestPlayer, 10000) << "\n";
	TicTacToe::playAgainstHuman(bestPlayer);*/
} 