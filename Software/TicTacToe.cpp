#include "TicTacToe.h"

net::NeuralNet TicTacToe::getBestPlayer(int numberOfIterations) {

}

std::vector<double> TicTacToe::getPlayerFitnesses(std::vector<net::NeuralNet> players) {
	std::vector<double> scores;
	for(int a = 0; a < players.size(); a++) {
		scores.push_back(2000);
	}

	for(int a = 0; a < players.size(); a++) {
		for(int b = a+1; b < players.size(); b++) {
			if(doesFirstPlayerWin(players[a], players[b])) {
				/// Change elo score to favor first player
			} else {
				/// Change elo score to favor second player
			}
		}
	}
}

bool TicTacToe::doesFirstPlayerWin(net::NeuralNet player1, net::NeuralNet player2) {
	std::vector< std::vector<int> > board;
	for(int a = 0; a < 3; a++) {
		std::vector<int> row;
		for(int b = 0; b < 3; b++) {
			row.push_back(0);
		}
		board.push_back(row);
	}

	while(hasGameEnded(board) == false) {
		/// do stuff adf;ljas;lkfj;alskjdf;laksjdf;lkasjdflkjas;fdlk
		
	}
}

bool TicTacToe::hasGameEnded(std::vector< std::vector<int> > board) {
	bool foundEmptySpace = false;
	for(int a = 0; a < board.size(); a++) for(int b = 0; b < board[a].size(); b++) if(board[a][b] == '0') foundEmptySpace = true;
	if(foundEmptySpace == false) return true;
	
	/// Check if someone filled up a row
	for(int a = 0; a < board.size(); a++) {
		int intialValue = board[a][0];
		bool isRowOneValue = true;
		for(int b = 1; b < board[a].size(); b++) {
			if(intialValue != board[a][b]) isRowOneValue = false;
		}
		if(isRowOneValue == true) return true;
	}
	
	/// Check if someone fille up a column
	for(int a = 0; a < board[0].size(); a++) {
		int intialValue = board[0][a];
		bool isColumOneValue = true;
		for(int b = 1; b < board.size(); b++) {
			if(intialValue != board[b][a]) isColumOneValue = false;
		}
		if(isColumOneValue == true) return true;
	}

	/// Check if someone filled a diagonal
	int intialValue = board[0][0];
	bool isDiagonalOneValue = true;
	for(int a = 1; a < board.size(); a++) if(board[a][a] == intialValue) isDiagonalOneValue = false;
	if(isDiagonalOneValue == true) return true;

	intialValue = board[0][board.size()-1];
	isDiagonalOneValue = true;
	for(int a = 1; a < board.size(); a++) if(board[a][(board.size() - 1) - a] == intialValue) isDiagonalOneValue = false;
	if(isDiagonalOneValue == true) return true;
	
	return false;
}

