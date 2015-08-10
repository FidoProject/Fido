#include "TicTacToe.h"

net::NeuralNet TicTacToe::getBestPlayer(int numberOfIterations) {
	gen::GeneticAlgo geneticAlgo(100, 0.5, 0, getPlayerFitnesses);

	net::NeuralNet modelNet(9, 1, 3, 9, net::NeuralNet::integer);

	return geneticAlgo.getBestNeuralNetwork(numberOfIterations, modelNet);
}

std::vector<double> TicTacToe::getPlayerFitnesses(std::vector<net::NeuralNet> players) {
	std::vector<double> scores;
	for(int a = 0; a < players.size(); a++) {
		scores.push_back(1000);
	}
	int numberMatchesPerPlayer = 20;

	/// Elo Rating System
	for(int a = 0; a < players.size(); a++) {
		for(int b = a + 1; (b+1) % numberMatchesPerPlayer != 0 && b < players.size(); b++) {
			//std::cout << "a " << (b-a) << "\n";
			net::NeuralNet player1 = players[a], player2 = players[b];
			std::cout << "w";
			/// If there was a win
			int outcome = getOutcomeOfGame(&player1, &player2);
			if(outcome == 1 || outcome == 2) {
				std::cout << "WTF";
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
    /*std::vector<double> weights1 = player1->getWeights(), weights2 = player2->getWeights();
    std::cout << "Weights 1: ";
    for (int a = 0; a < weights1.size(); a++) {
        std::cout << weights1[a] << ", ";
    }
    std::cout << "\n";
    
    std::cout << "Weights 2: ";
    for (int a = 0; a < weights2.size(); a++) {
        std::cout << weights2[a];
    }
    std::cout << "\n";*/
    
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
        didOneMakeMove = false;
        
		int output1 = (int)(player1->getOutput(prepareBoardForPlayerInput(board, 1)))[0] % 9;
		int p1Row = floor(output1 / 3.0), p1Colum = output1 % 3;
        //std::cout << "x, y: " << p1X << ", " << p1Y << "\n";
		if(board[p1Row][p1Colum] == 0) {
            didOneMakeMove = true;
			board[p1Row][p1Colum] = 1;
        }
        
        if(hasGameEnded(board) == true) break;
        
		int output2 = (int)(player2->getOutput(prepareBoardForPlayerInput(board, 2)))[0] % 9;
		int p2Row = floor(output2 / 3.0), p2Colum = output2 % 3;
        //std::cout << "x, y: " << p2X << ", " << p2Y << "\n";
		if(board[p2Row][p2Colum] == 0) {
            didOneMakeMove = true;
			board[p2Row][p2Colum] = 2;
        }
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
		didOneMakeMove = false;

		int output1 = (int)(player1->getOutput(prepareBoardForPlayerInput(board, 1)))[0] % 9;
		int p1Row = floor(output1 / 3.0), p1Colum = output1 % 3;
		if(board[p1Row][p1Colum] == 0) {
			didOneMakeMove = true;
			board[p1Row][p1Colum] = 1;
		}
        
        printBoard(board);
        std::cout << "\n";
        
		if(hasGameEnded(board) == true) break;

		int output2 = (int)(player2->getOutput(prepareBoardForPlayerInput(board, 2)))[0] % 9;
		int p2Row = floor(output2 / 3.0), p2Colum = output2 % 3;
		if(board[p2Row][p2Colum] == 0) {
			didOneMakeMove = true;
			board[p2Row][p2Colum] = 2;
		}
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
		int output1 = (int)(player->getOutput(prepareBoardForPlayerInput(board, 1)))[0] % 9;
		int p1Row = floor(output1 / 3.0), p1Colum = output1 % 3;
		if(board[p1Row][p1Colum] == 0)board[p1Row][p1Colum] = 1;
        
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
