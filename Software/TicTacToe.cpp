#include "TicTacToe.h"

net::NeuralNet TicTacToe::getBestPlayer(int numberOfIterations) {
	gen::GeneticAlgo geneticAlgo(20, 0.5, 0, getPlayerFitnesses);

	net::NeuralNet modelNet(9, 1, 3, 9, net::NeuralNet::sigmoid);

	return geneticAlgo.getBestNeuralNetwork(numberOfIterations, modelNet);
}

std::vector<double> TicTacToe::getPlayerFitnesses(std::vector<net::NeuralNet> players) {
	std::vector<double> scores;
	for(int a = 0; a < players.size(); a++) {
		scores.push_back(1000);
	}
	int numberMatchesPerPlayer = 40;

	/// Elo Rating System
	for(int a = 0; a < players.size(); a++) {
		for(int b = a + 1; (b+1) % numberMatchesPerPlayer != 0 && b < players.size(); b++) {
			//std::cout << "a " << (b-a) << "\n";
			net::NeuralNet player1 = players[a], player2 = players[b];
			//std::cout << "w";
			/// If there was a win
			//playVisualGame(&player1, &player2);
			int outcome = getOutcomeOfGame(&player1, &player2);
			if(outcome == 1 || outcome == 2) {
				//std::cout << "WTF";
				double p1Points = 0, p2Points = 0;
				double p1ExpectedPoints = 1 / (1 + pow(10, (scores[b] - scores[a]) / 400));
				double p2ExpectedPoints = 1 / (1 + pow(10, (scores[a] - scores[b]) / 400));
				double kfactor = 32;
				if(outcome == 1) {
					p1Points = 1;
				} else if(outcome == 2){
					p2Points = 1;
				}
				scores[a] += 100 * (p1Points - p1ExpectedPoints);
				scores[b] += 100 * (p2Points - p2ExpectedPoints);
			}
		}
	}

	for(int a = 0; a < scores.size(); a++) scores[a] = pow(scores[a], 1.1);

	/*for(int a = 0; a < players.size(); a++) {
		for(int b = a + 1; b < players.size(); b++) {
            net::NeuralNet player1 = players[a], player2 = players[b];
			int outcome = getOutcomeOfGame(&player1, &player2);
			//playVisualGame(&player1, &player2);
            if(outcome == 1) {
				scores[a]++;
            } else if(outcome == 2){
				scores[b]++;
            }
		}
	}*/

	return scores;
}

int TicTacToe::getOutcomeOfGame(net::NeuralNet *player1, net::NeuralNet *player2) {
    
    std::vector< std::vector<int> > board;
	for(int a = 0; a < 3; a++) {
		std::vector<int> row;
		for(int b = 0; b < 3; b++) {
			row.push_back(0);
		}
		board.push_back(row);
	}  
    
	while(hasGameEnded(board) == false) {
		executeMove(player1, board, 1);

        if(hasGameEnded(board) == true) break;
        
		executeMove(player1, board, 2);
	}
    //printBoard(board);

	/// Check if someone filled up a row
	for(int a = 0; a < board.size(); a++) {
		int initialValue = board[a][0];
		bool isRowOneValue = true;
		for(int b = 1; b < board[a].size(); b++) {
			if(initialValue != board[a][b]) isRowOneValue = false;
		}
		if(isRowOneValue == true) {
			if(initialValue == 1) return 1;
			else if(initialValue == 2) return 2;
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
			else if(initialValue == 2) return 2;
		}
	}

	/// Check if someone filled a diagonal
	int initialValue = board[0][0];
	bool isDiagonalOneValue = true;
	for(int a = 1; a < board.size(); a++) if(board[a][a] != initialValue) isDiagonalOneValue = false;
	if(isDiagonalOneValue == true) {
		if(initialValue == 1) return 1;
		else if(initialValue == 2) return 2;
	}

	initialValue = board[0][board.size()-1];
	isDiagonalOneValue = true;
	for(int a = 1; a < board.size(); a++) if(board[a][(board.size() - 1) - a] != initialValue) isDiagonalOneValue = false;
	if(isDiagonalOneValue == true) {
		if(initialValue == 1) return 1;
		else if(initialValue == 2) return 2;
	}
    
    //std::cout << "Reached end\n";
	return 0;
}

void TicTacToe::executeMove(net::NeuralNet *player, std::vector< std::vector<int> > &board, int playerNumber) {
	int bestRow = 0, bestColum = 0;
	int bestScore = -9999999999;

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

bool TicTacToe::hasGameEnded(std::vector< std::vector<int> > board) {
    //std::cout << "1\n";
	bool foundEmptySpace = false;
    for(int a = 0; a < board.size(); a++) for(int b = 0; b < board[a].size(); b++) if(board[a][b] == 0) foundEmptySpace = true;
	if(foundEmptySpace == false) return true;
	
    //std::cout << "2\n";
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
    
    //std::cout << "3\n";
	
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
    //std::cout << "4\n";

	/// Check if someone filled a diagonal
	int initialValue = board[0][0];
    if(initialValue != 0) {
        bool isDiagonalOneValue = true;
        for(int a = 1; a < board.size(); a++) if(board[a][a] != initialValue) isDiagonalOneValue = false;
        if(isDiagonalOneValue == true) return true;
    }
    //std::cout << "5\n";
	initialValue = board[0][board.size()-1];
	if(initialValue != 0) {
        bool isDiagonalOneValue = true;
        for(int a = 1; a < board.size(); a++) if(board[a][(board.size() - 1) - a] != initialValue) isDiagonalOneValue = false;
        if(isDiagonalOneValue == true) return true;
    }
	//std::cout << "6\n";
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

void TicTacToe::playVisualGame(net::NeuralNet *player1, net::NeuralNet *player2) {
    std::vector< std::vector<int> > board;
    for(int a = 0; a < 3; a++) {
        std::vector<int> row;
        for(int b = 0; b < 3; b++) {
            row.push_back(0);
        }
        board.push_back(row);
    }
    
	bool didOneMakeMove = true;

    while(hasGameEnded(board) == false && didOneMakeMove == true) {
		executeMove(player1, board, 1);
		printBoard(board);

		if(hasGameEnded(board) == true) break;

		executeMove(player1, board, 2);
		printBoard(board);
    }
}

void TicTacToe::playAgainstHuman(net::NeuralNet *player) {
    std::vector< std::vector<int> > board;
    for(int a = 0; a < 3; a++) {
        std::vector<int> row;
        for(int b = 0; b < 3; b++) {
            row.push_back(0);
        }
        board.push_back(row);
    }
    
    while(hasGameEnded(board) == false) {
		executeMove(player, board, 1);
		printBoard(board);
        
        if(hasGameEnded(board) == true) break;
        
        int row, colum;
        std::cout << "Enter your row: ";
        std::cin >> row;
        std::cout << "Enter your colum: ";
        std::cin >> colum;
        
        if(board[row][colum] == 0) board[row][colum] = 2;
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
    
    //for(int a = 0; a < returnVector.size(); a++) std::cout << returnVector[a] << ", ";
    //std::cout << "\n";
    return returnVector;
}
