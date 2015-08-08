#ifndef GENETICALGO_H
#define GENETICALGO_H

#include "NeuralNet.h"
#include "GeneticAlgo.h"

class TicTacToe
{
public:
	net::NeuralNet getBestProgramNetwork(int numberOfIterations);
	std::vector<double> getPlayerFitnesses(std::vector<net::NeuralNet> players);
	bool doesFirstPlayerWin(net::NeuralNet player1, net::NeuralNet player2);
	bool hasGameEnded(std::vector< std::vector<int> > board);
};

#endif