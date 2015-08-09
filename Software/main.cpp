#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Neuron.h"
#include "NeuralNet.h"
#include "GeneticAlgo.h"
#include "TicTacToe.h"

int main() {
    srand(time(NULL));
	std::cout << "Start\n";
	TicTacToe game;
	std::cout << "Finished intialization\n";
	net::NeuralNet bestPlayer = game.getBestPlayer(400);
	std::cout << "Done\n";
    net::NeuralNet secondBestPlayer = game.getBestPlayer(200);
    //TicTacToe::playVisualGame(bestPlayer, secondBestPlayer);
    std::cout << "DONE\n";
}