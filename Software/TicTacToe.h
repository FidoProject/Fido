#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "NeuralNet.h"
#include "GeneticAlgo.h"

class TicTacToe
{
public:
	// Get the neural network representing the best performing tic tac toe player from a population that has undergone a specified number of iterations
	net::NeuralNet getBestPlayer(int numberOfIterations);

	// Gets the fitness of each neural network in a population of neural networks
	static std::vector<double> getPlayerFitnesses(std::vector<net::NeuralNet> players);

	// Pits two neural networks against each other in tic tac toe and returns 0 if there is a tie, 1 if player1 wins, and 2 if player2 wins
	static int getOutcomeOfGame(net::NeuralNet player1, net::NeuralNet player2);

	/* 
	 * Determines if a tic tac toe game has ended given a 2d vector of ints representing the board
	 *
	 * In the board, O represents an empty sapce, 1 represents the first player's piece, and 2 represents a second player's piece
	 */
	static bool hasGameEnded(std::vector< std::vector<int> > board);

	// Print a tic tac toe board consisting of 0s (representing an empty space), 1s (representing the first player's piece), and 2 (representing a second player's piece)
	static void printBoard(std::vector< std::vector<int> > board);
};

#endif