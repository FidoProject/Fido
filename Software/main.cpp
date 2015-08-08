#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Neuron.h"
#include "NeuralNet.h"
#include "GeneticAlgo.h"
#include "TicTacToe.h"

int main() {
	std::cout << "Start\n";
	TicTacToe game;
	std::cout << "Finished intialization\n";
	net::NeuralNet bestPlayer = game.getBestPlayer(2000);
	std::cout << "Done\n";
    net::NeuralNet secondBestPlayer = game.getBestPlayer(1000);
}