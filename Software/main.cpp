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

    net::NeuralNet bestPlayer("nn10.txt", net::NeuralNet::sigmoid);
    //bestPlayer.storeNet("nn10.txt");
	TicTacToe::playAgainstHuman(&bestPlayer);

    //net::NeuralNet secondBestPlayer = game.getBestPlayer(10);
    //secondBestPlayer.storeNet("Documents/Fido/Software/nn10.txt");

    //int outcome = TicTacToe::getOutcomeOfGame(&bestPlayer, &secondBestPlayer);
    //std::cout << "Outcome: " << outcome << "\n";

    std::cout << "DONE\n";
}