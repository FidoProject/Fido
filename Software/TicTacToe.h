#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <chrono>
#include <thread>

#include "NeuralNet.h"
#include "GeneticAlgo.h"

class TicTacToe
{
public:
	// Get the neural network representing the best performing tic tac toe player from a population that has undergone a specified number of iterations
	net::NeuralNet* getBestPlayer(int numberOfIterations);

	// Gets the fitness of each neural network in a population of neural networks
	static std::vector<double> getPlayerFitnesses(std::vector<net::NeuralNet *> players);

	// Pits two neural networks against each other in tic tac toe and returns 0 if there is a tie, 1 if player1 wins, and 2 if player2 wins
	static int getOutcomeOfGame(net::NeuralNet *player1, net::NeuralNet *player2);

	// Gets the neural network's move in tic tac toe and executes it on the board
	static void executeMove(net::NeuralNet *player, std::vector< std::vector<int> > &board, int playerNumber);

	// Initializes the board
	static void initializeBoard(std::vector< std::vector<int> > &board);

	/* 
	 * Determines if a tic tac toe game has ended given a 2d vector of ints representing the board
	 *
	 * In the board, O represents an empty sapce, 1 represents the first player's piece, and 2 represents a second player's piece
	 */
	static bool hasGameEnded(std::vector< std::vector<int> > board);

	// Print a tic tac toe board consisting of 0s (representing an empty space), 1s (representing the first player's piece), and -1s (representing a second player's piece)
	static void printBoard(std::vector< std::vector<int> > board);
    
    // Pits two neural networks against each other in tic tac toe and shows each of their moves in the command line
    static void playVisualGame(net::NeuralNet *player1, net::NeuralNet *player2);
    
    // Pits a human and a neural network against each other in tic tac toe using the command line
    static void playAgainstHuman(net::NeuralNet *player);
    
    // Takes in a 2-d vector that represents the game board turns it into a suitable input for a neural network
    static std::vector<double> prepareBoardForPlayerInput(std::vector< std::vector<int> > board, int playerNumber);

	// Pits a neural network against random neural networks in tic tac toe and returns the number of wins
	static int getWinsAgainstRandomPlayers(net::NeuralNet *player, int numberOfGames);

	// Pits a neural network against a set of neural networks in tic tac toe and returns the number of wins
	static int getWinsAgainstSetOfPlayers(net::NeuralNet *player, std::vector<net::NeuralNet *> networks);

	// A random set of neural networks used for evaluating the skill of other neural networks at TicTacToe
	static std::vector<net::NeuralNet *> randomSet;
};

#endif