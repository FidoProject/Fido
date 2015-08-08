#include "TicTacToe.h"

net::NeuralNet TicTacToe::getBestPlayer(int numberOfIterations) {
	gen::GeneticAlgo geneticAlgo(20, 0.2, 0.7, getPlayerFitnesses);

	net::NeuralNet modelNet(9, 2, 2, 6, net::NeuralNet::sigmoid);

	return geneticAlgo.getBestNeuralNetwork(numberOfIterations, modelNet);
}

std::vector<double> TicTacToe::getPlayerFitnesses(std::vector<net::NeuralNet> players) {
	std::vector<double> wins;
	for(int a = 0; a < players.size(); a++) {
		wins.push_back(0);
	}

	for(int a = 0; a < players.size(); a++) {
		for(int b = a+1; b < players.size(); b++) {
			int outcome = getOutcomeOfGame(players[a], players[b]);
			if(outcome == 1) wins[a]++;
			else if(outcome == 2) wins[b]++;
		}
	}
	return wins;
}

int TicTacToe::getOutcomeOfGame(net::NeuralNet player1, net::NeuralNet player2) {
	std::vector< std::vector<int> > board;
	for(int a = 0; a < 3; a++) {
		std::vector<int> row;
		for(int b = 0; b < 3; b++) {
			row.push_back(0);
		}
		board.push_back(row);
	}

	while(hasGameEnded(board) == false) {
		std::vector<double> oneDBoard;
		for(int a = 0; a < board.size(); a++) for(int b = 0; b < board[a].size(); b++) oneDBoard.push_back(board[a][b]);

		std::vector<double> coordinates1 = player1.getOutput(oneDBoard);
		int p1X = ((int)coordinates1[0]) % 3, p1Y = ((int)coordinates1[1]) % 3;
		if(board[p1X][p1Y] == 0) board[p1X][p1Y] = 1;

		std::vector<double> coordinates2 = player2.getOutput(oneDBoard);
		int p2X = ((int)coordinates2[0]) % 3, p2Y = ((int)coordinates2[1]) % 3;
		if(board[p2X][p2Y] == 0) board[p2X][p2Y] = 2;
	}

	/// Check if someone filled up a row
	for(int a = 0; a < board.size(); a++) {
		int intialValue = board[a][0];
		bool isRowOneValue = true;
		for(int b = 1; b < board[a].size(); b++) {
			if(intialValue != board[a][b]) isRowOneValue = false;
		}
		if(isRowOneValue == true) {
			if(intialValue == 1) return 1;
			else return 2;
		}
	}
	
	/// Check if someone filled up a column
	for(int a = 0; a < board[0].size(); a++) {
		int intialValue = board[0][a];
		bool isColumOneValue = true;
		for(int b = 1; b < board.size(); b++) {
			if(intialValue != board[b][a]) isColumOneValue = false;
		}
		if(isColumOneValue == true) {
			if(intialValue == 1) return 1;
			else return 2;
		}
	}

	/// Check if someone filled a diagonal
	int intialValue = board[0][0];
	bool isDiagonalOneValue = true;
	for(int a = 1; a < board.size(); a++) if(board[a][a] == intialValue) isDiagonalOneValue = false;
	if(isDiagonalOneValue == true) {
		if(intialValue == 1) return 1;
		else return 2;
	}

	intialValue = board[0][board.size()-1];
	isDiagonalOneValue = true;
	for(int a = 1; a < board.size(); a++) if(board[a][(board.size() - 1) - a] == intialValue) isDiagonalOneValue = false;
	if(isDiagonalOneValue == true) {
		if(intialValue == 1) return 1;
		else return 2;
	}

	return 0;
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
	
	/// Check if someone filled up a column
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

void TicTacToe::printBoard(std::vector< std::vector<int> > board) {
	for(int a = 0; a < board.size(); a++) {
		for(int b = 0; b < board[a].size(); b++) {
			if(board[a][b] == 0) std::cout << "| ";
			else if(board[a][b] == 1) std::cout << "|X";
			else if(board[a][b] == 2) std::cout << "|O";
		}
		std::cout << "|\n";
	}
}

