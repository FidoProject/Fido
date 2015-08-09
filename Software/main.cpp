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
	net::NeuralNet bestPlayer = game.getBestPlayer(200);
	std::cout << "Done\n";
    //net::NeuralNet secondBestPlayer = game.getBestPlayer(50);
    TicTacToe::playAgainstHuman(&bestPlayer);
    std::cout << "DONE\n";
}