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
    net::NeuralNet bestPlayer("/Users/michaeltruell/Documents/Fido/Software/storage.txt", net::NeuralNet::integer);
    TicTacToe::playAgainstHuman(&bestPlayer);
    bestPlayer.storeNet("/Users/michaeltruell/Documents/Fido/Software/storage.txt");
    std::cout << "DONE\n";
}