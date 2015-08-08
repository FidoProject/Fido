#ifndef GENETICALGO_H
#define GENETICALGO_H

#include "NeuralNet.h"
#include "GeneticAlgo.h"

class TicTacToe
{
public:
	// Get the neural network representing the best performing tic tac toe player from a population that has undergone a specified number of iterations
	net::NeuralNet getBestPlayer(int numberOfIterations);
	
	// Gets the fitness of each neural network in a population of neural networks
	std::vector<double> getPlayerFitnesses(std::vector<net::NeuralNet> players);

	// Pits two neural networks against each other in tic tac toe and determines if player1 wins or loses
	bool doesFirstPlayerWin(net::NeuralNet player1, net::NeuralNet player2);

	/* 
	 * Determines if a tic tac toe game has ended given a 2d vector of ints representing the board
	 *
	 * In the board, O represents an empty sapce, 1 represents the first player's piece, and two represents a second player' piece
	 */
	bool hasGameEnded(std::vector< std::vector<int> > board);
};

#endif