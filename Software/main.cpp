#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>

#include "Neuron.h"
#include "NeuralNet.h"
#include "GeneticAlgo.h"
#include "TicTacToe.h"
#include "Backpropagation.h"
#include "QLearn.h"


int main() {
    srand(time(NULL));

	net::QLearn learn = net::QLearn(new net::NeuralNet(3, 1, 3, 3, net::NeuralNet::sigmoid), net::Backpropagation(0.5, 0.9, 0.01, 100000), 1, 0.5, 2);
	std::vector<double> state(1);
	double reward;
	for(int a = 0; a < 100; a++) {
		int determiner = a % 2;

		state[0] = determiner;
		int action = learn.chooseAction(state);
		state[0] = (determiner + 1) % 2;
		if(action != determiner) learn.applyReinforcement(-0.1, state);
		else learn.applyReinforcement(1, state);
	}

	std::cout << "input plz\n";
	std::cin >> state[0];
	while(true) {
		std::cout << "Action taken: " << learn.chooseAction(state) << "\n";
		std::cout << "reward plz\n";
		std::cin >> reward;
		std::cout << "new state plz\n";
		std::cin >> state[0];
		learn.applyReinforcement(reward, state);
	}

} 