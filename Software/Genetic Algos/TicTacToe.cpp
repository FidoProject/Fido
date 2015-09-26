#include "TicTacToe.h"

#include <thread>
#include <iostream>

#include "GeneticAlgo.h"

std::vector<net::NeuralNet *> TicTacToe::randomSet = std::vector<net::NeuralNet*>(0);

net::NeuralNet* TicTacToe::getBestPlayer(int numberOfIterations) {
	gen::GeneticAlgo geneticAlgo(8, 0.2, 0.6, 5, getPopulationFitnesses);

	net::NeuralNet *modelNet = new net::NeuralNet(9, 1, 4, 9, "sigmoid");

	///for(int a = 0; a < 250; a++) randomSet.push_back(new net::NeuralNet(9, 1, 4, 9, "sigmoid"));

	return geneticAlgo.getBestNeuralNetwork(numberOfIterations, modelNet);
}

std::vector<double> TicTacToe::getPopulationFitnesses(std::vector<net::NeuralNet *> players) {
	std::vector< std::vector<double>* > scores;
	std::vector<std::thread> threads;
	int numThreads = 4;

	int sizePerSubgroup = (int)players.size() / numThreads;

	for(int a = 0; a < numThreads; a++) {
		scores.push_back(new std::vector<double>(0));
		if(a == numThreads-1) {
			threads.push_back(std::thread(getPlayerFitnessesThread, std::vector<net::NeuralNet *>(players.begin() + sizePerSubgroup*a, players.end()), scores[a]));
		} else {
			threads.push_back(std::thread(getPlayerFitnessesThread, std::vector<net::NeuralNet *>(players.begin() + sizePerSubgroup*a, players.begin() + sizePerSubgroup*(a + 1)), scores[a]));
		}
	}
	for(int a = 0; a < numThreads; a++) threads[a].join();
	
	std::vector<double> returnVector;
	for(int a = 0; a < scores.size(); a++) for(int b = 0; b < scores[a]->size(); b++) returnVector.push_back((*scores[a])[b]);

	int mostFitIndex = 0;
	for(int a = 0; a < returnVector.size(); a++) if(returnVector[a] > returnVector[mostFitIndex]) mostFitIndex = a;
	players[mostFitIndex]->storeNet("C:/Users/Michael Truell/Documents/Fido/Software/PastNN/TTTcurrentgenerationbest.txt");

	return returnVector;
}

void TicTacToe::getPlayerFitnessesThread(std::vector<net::NeuralNet *> players, std::vector<double> *fitnesses) {
	/// Elo Rating System
	for(int a = 0; a < players.size(); a++) {
		double score = TicTacToe::getScoreAgainstRandomPlayers(players[a], 40);
		std::cout << "Score: " << score << "\n";
		fitnesses->push_back(score);
	}

	for(int a = 0; a < fitnesses->size(); a++) (*fitnesses)[a] = pow((*fitnesses)[a], 2);
}

int TicTacToe::getOutcomeOfGame(net::NeuralNet *player1, net::NeuralNet *player2) {
    std::vector< std::vector<int> > board;
	initializeBoard(board);
    
	int timesAround = 0;
	while(timesAround < 2) {
		executeMove(player1, board, 1);
		executeMove(player2, board, -1);
		timesAround++;
	}
	while(hasGameEnded(board) == false) {
		executeMove(player1, board, 1);
		if(hasGameEnded(board) == true) break;
		executeMove(player2, board, -1);
	}

	/// Check if someone filled up a row
	for(int a = 0; a < board.size(); a++) {
		int initialValue = board[a][0];
		bool isRowOneValue = true;
		for(int b = 1; b < board[a].size(); b++) {
			if(initialValue != board[a][b]) isRowOneValue = false;
		}
		if(isRowOneValue == true) {
			if(initialValue == 1) return 1;
			else if(initialValue == -1) return 2;
		}
	}
	
	/// Check if someone filled up a column
	for(int a = 0; a < board[0].size(); a++) {
		int initialValue = board[0][a];
		bool isColumOneValue = true;
		for(int b = 1; b < board.size(); b++) {
			if(initialValue != board[b][a]) isColumOneValue = false;
		}
		if(isColumOneValue == true) {
			if(initialValue == 1) return 1;
			else if(initialValue == -1) return 2;
		}
	}

	/// Check if someone filled a diagonal
	int initialValue = board[0][0];
	bool isDiagonalOneValue = true;
	for(int a = 1; a < board.size(); a++) if(board[a][a] != initialValue) isDiagonalOneValue = false;
	if(isDiagonalOneValue == true) {
		if(initialValue == 1) return 1;
		else if(initialValue == -1) return 2;
	}

	initialValue = board[0][board.size()-1];
	isDiagonalOneValue = true;
	for(int a = 1; a < board.size(); a++) if(board[a][(board.size() - 1) - a] != initialValue) isDiagonalOneValue = false;
	if(isDiagonalOneValue == true) {
		if(initialValue == 1) return 1;
		else if(initialValue == -1) return 2;
	}
    
	return 0;
}

void TicTacToe::executeMove(net::NeuralNet *player, std::vector< std::vector<int> > &board, int playerNumber) {
	int bestRow = 0, bestColum = 0;
	double bestScore = -9999999999;

	std::vector<double> oneDBoard;
	for(int a = 0; a < board.size(); a++) {
		for(int b = 0; b < board[a].size(); b++) {
			if(board[a][b] == 0) oneDBoard.push_back(0);
			else if(board[a][b] == playerNumber) oneDBoard.push_back(1);
			else oneDBoard.push_back(-1);
		}
	}

	for(int a = 0; a < oneDBoard.size(); a++) {
		if(oneDBoard[a] == 0) {
			oneDBoard[a] = 1;
			double score = player->getOutput(oneDBoard)[0];
			if(score > bestScore) {
				bestScore = score;
				bestRow = floor(a / 3);
				bestColum = a % 3;
			}
			oneDBoard[a] = 0;
		}
	}
	board[bestRow][bestColum] = playerNumber;
}

void TicTacToe::initializeBoard(std::vector< std::vector<int> > &board) {
	for(int a = 0; a < 3; a++) {
		std::vector<int> row;
		for(int b = 0; b < 3; b++) row.push_back(0);
		board.push_back(row);
	}
}

bool TicTacToe::hasGameEnded(std::vector< std::vector<int> > board) {
	bool foundEmptySpace = false;
    for(int a = 0; a < board.size(); a++) for(int b = 0; b < board[a].size(); b++) if(board[a][b] == 0) foundEmptySpace = true;
	if(foundEmptySpace == false) return true;
	
	/// Check if someone filled up a row
	for(int a = 0; a < board.size(); a++) {
		int initialValue = board[a][0];
        if(initialValue != 0) {
            bool isRowOneValue = true;
            for(int b = 1; b < board[a].size(); b++) {
                if(initialValue != board[a][b]) isRowOneValue = false;
            }
            if(isRowOneValue == true) return true;
        }
	}
    
	
	/// Check if someone filled up a column
	for(int a = 0; a < board[0].size(); a++) {
		int initialValue = board[0][a];
        if(initialValue != 0) {
            bool isColumOneValue = true;
            for(int b = 1; b < board.size(); b++) {
                if(initialValue != board[b][a]) isColumOneValue = false;
            }
            if(isColumOneValue == true) return true;
        }
	}

	/// Check if someone filled up a diagonal
	int initialValue = board[0][0];
    if(initialValue != 0) {
        bool isDiagonalOneValue = true;
        for(int a = 1; a < board.size(); a++) if(board[a][a] != initialValue) isDiagonalOneValue = false;
        if(isDiagonalOneValue == true) return true;
    }
	initialValue = board[0][board.size()-1];
	if(initialValue != 0) {
        bool isDiagonalOneValue = true;
        for(int a = 1; a < board.size(); a++) if(board[a][(board.size() - 1) - a] != initialValue) isDiagonalOneValue = false;
        if(isDiagonalOneValue == true) return true;
    }
	return false;
}

void TicTacToe::printBoard(std::vector< std::vector<int> > board) {
	for(int a = 0; a < board.size(); a++) {
		for(int b = 0; b < board[a].size(); b++) {
			if(board[a][b] == 0) std::cout << "| ";
			else if(board[a][b] == 1) std::cout << "|X";
			else if(board[a][b] == -1) std::cout << "|O";
		}
		std::cout << "|\n";
	}
}

void TicTacToe::playVisualGame(net::NeuralNet *player1, net::NeuralNet *player2) {
    std::vector< std::vector<int> > board;
	initializeBoard(board);
    
	bool didOneMakeMove = true;

    while(hasGameEnded(board) == false && didOneMakeMove == true) {
		executeMove(player1, board, 1);
		printBoard(board);
		std::cout << "\n";

		if(hasGameEnded(board) == true) break;

		executeMove(player2, board, -1);
		printBoard(board);
		std::cout << "\n";
    }
}

void TicTacToe::playAgainstHuman(net::NeuralNet *player) {
    std::vector< std::vector<int> > board;
	initializeBoard(board);
    
    while(hasGameEnded(board) == false) {
		executeMove(player, board, 1);
		printBoard(board);
        
        if(hasGameEnded(board) == true) break;
        
        int row, colum;
        std::cout << "Enter your row: ";
        std::cin >> row;
        std::cout << "Enter your colum: ";
        std::cin >> colum;
        if(board[row][colum] == 0) board[row][colum] = -1;
        printBoard(board);
    }
}

std::vector<double> TicTacToe::prepareBoardForPlayerInput(std::vector< std::vector<int> > board, int playerNumber) {
    std::vector<double> returnVector;
    for(int a = 0; a < board.size(); a++) {
        for(int b = 0 ; b < board[a].size(); b++) {
            if(board[a][b] == 0) returnVector.push_back(0);
            else if(board[a][b] == playerNumber) returnVector.push_back(1);
            else returnVector.push_back(-1);
        }
    }

    return returnVector;
}

double TicTacToe::getScoreAgainstRandomPlayers(net::NeuralNet *player, int numberOfGames) {
	double score = 0;
	for(int a = 0; a < numberOfGames; a++) {
		net::NeuralNet random = net::NeuralNet(9, 1, 4, 12, "sigmoid");
		if(a < (int)(numberOfGames / 2.0)) {
			int outcome = getOutcomeOfGame(player, &random);
			if(outcome == 1) score++;
			else if(outcome == 0) score += 0.5;
		} else {
			int outcome = getOutcomeOfGame(&random, player);
			if(outcome == 2) score++;
			else if(outcome == 0) score += 0.5;
		}
	}
	return score;
}

// Pits a neural network against a set of neural networks in tic tac toe and returns the number of wins
double TicTacToe::getScoreAgainstSetOfPlayers(net::NeuralNet *player, std::vector<net::NeuralNet *> networks) {
	double score = 0;
	for(int a = 0; a < networks.size(); a++) {
		if(a < (int)(networks.size() / 2.0)) {
			int outcome = getOutcomeOfGame(player, networks[a]);
			if(outcome == 1) score++;
			else if(outcome == 0) score += 0.5;
		} else {
			int outcome = getOutcomeOfGame(networks[a], player);
			if(outcome == 2) score++;
			else if(outcome == 0) score += 0.5;
		}
	}
	return score;
}