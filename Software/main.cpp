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

	net::NeuralNet *bestPlayer = TicTacToe::getBestPlayer(100);

	/*net::NeuralNet * network = new net::NeuralNet(1, 1, 2, 2, net::NeuralNet::sigmoid);
	network->outputActivationFunction = net::NeuralNet::simpleLinear;

	net::Backpropagation backprop = net::Backpropagation(0.1, 0.05, 0.01, 100000);
	backprop.outputActivationFunctionDerivative = net::Backpropagation::simpleLinearDerivative;

	net::QLearn learn = net::QLearn(network, backprop, 0.3, 0.5, 2);

	std::vector<double> state(1);
	state[0] = 0;
	double reward;

	for(int a = 0; a < 2000; a++) {
		int action = learn.chooseBoltzmanAction(state, 0.5);
		std::cout << "action: " << action << "; state: " << state[0] << "\n";
		if(action != (int)state[0]) {
			learn.applyReinforcementToLastAction(-1, state);
		} else  {
			state[0] = (int)(state[0]+1) % 2;
			learn.applyReinforcementToLastAction(1, state);
		}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	
	bool done = false;
	while(done == false) {
		done = true;
		int action = learn.chooseBestAction(state);
		if(action != (int)state[0]) {
			learn.applyReinforcementToLastAction(-1, state);
			done = false;
		} else  {
			state[0] = (int)(state[0] + 1) % 2;
			learn.applyReinforcementToLastAction(1, state);
		}
		
		action = learn.chooseBestAction(state);
		if(action != (int)state[0]) {
			learn.applyReinforcementToLastAction(-1, state);
			done = false;
		} else  {
			state[0] = (int)(state[0] + 1) % 2;
			learn.applyReinforcementToLastAction(1, state);
		}

	}

	std::cout << "input plz\n";
	std::cin >> state[0];
	while(true) {
		std::cout << "Action taken: " << learn.chooseBestAction(state) << "\n";
		std::cout << "reward plz\n";
		std::cin >> reward;
		std::cout << "new state plz\n";
		std::cin >> state[0];
		learn.applyReinforcementToLastAction(reward, state);
	}*/

} 