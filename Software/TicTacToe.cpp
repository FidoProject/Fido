#include "TicTacToe.h"

std::vector<net::NeuralNet *> TicTacToe::randomSet = std::vector<net::NeuralNet*>(0);

net::NeuralNet* TicTacToe::getBestPlayer(int numberOfIterations) {
	gen::GeneticAlgo geneticAlgo(5, 0.2, 0.6, 1, getPlayerFitnesses);

	net::NeuralNet modelNet(9, 1, 3, 9, net::NeuralNet::sigmoid);

	for(int a = 0; a < 100; a++) randomSet.push_back(new net::NeuralNet(9, 1, 3, 9, net::NeuralNet::sigmoid));

	return geneticAlgo.getBestNeuralNetwork(numberOfIterations, modelNet);
}

std::vector<double> TicTacToe::getPlayerFitnesses(std::vector<net::NeuralNet *> players) {
	std::vector<double> scores;
	/// Elo Rating System
	for(int a = 0; a < players.size(); a++) {
		double score = TicTacToe::getWinsAgainstSetOfPlayers(players[a], randomSet);
		std::cout << "Score: " << score << "\n";
		scores.push_back(score);
	}

	//for(int a = 0; a < scores.size(); a++) scores[a] = pow(scores[a], 2);

	return scores;
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
    
    //std::cout << "Reached end\n";
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

int TicTacToe::getWinsAgainstRandomPlayers(net::NeuralNet *player, int numberOfGames) {
	int numberOfWins = 0;
	for(int a = 0; a < numberOfGames; a++) {
		net::NeuralNet random = net::NeuralNet(9, 1, 3, 9, net::NeuralNet::sigmoid);
		if(a < (int)(numberOfGames/2.0)) {
			if(getOutcomeOfGame(player, &random) == 1) numberOfWins++;
		} else {
			if(getOutcomeOfGame(&random, player) == 2) numberOfWins++;
		}
	}
	return numberOfWins;
}

// Pits a neural network against a set of neural networks in tic tac toe and returns the number of wins
int TicTacToe::getWinsAgainstSetOfPlayers(net::NeuralNet *player, std::vector<net::NeuralNet *> networks) {
	int numberOfWins = 0;
	for(int a = 0; a < networks.size(); a++) {
		if(a < (int)(networks.size() / 2.0)) {
			if(getOutcomeOfGame(player, networks[a]) == 1) numberOfWins++;
		} else {
			if(getOutcomeOfGame(networks[a], player) == 2) numberOfWins++;
		}
	}
	return numberOfWins;
}