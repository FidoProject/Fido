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
#include "WireFitQLearn.h"


int main() {
    srand(time(NULL));
    
    net::NeuralNet * network = new net::NeuralNet(1, 1, 1, 2, "sigmoid");
	network->setOutputActivationFunction("simpleLinear");

	net::Backpropagation backprop = net::Backpropagation(0.2, 0, 0.001, 10000);
	backprop.setDerivedOutputActivationFunction("simpleLinearDerivative");

	net::QLearn learn = net::QLearn(network, backprop, 0.9, 0.5, 2);

	std::vector<double> state(1);
	state[0] = 0;
	double reward;
    double explorationConstant = 5;

	/*for(int a = 0; a < 2000; a++) {
		int action = learn.chooseBoltzmanAction(state, 0.5);
		std::cout << "action: " << action << "; state: " << state[0] << "\n";
		if(action != (int)state[0]) {
			learn.applyReinforcementToLastAction(-1, state);
		} else  {
			state[0] = (int)(state[0]+1) % 2;
			learn.applyReinforcementToLastAction(1, state);
		}
	}*/
    int index = 0;
	bool done = false;
	while(done == false) {
        index++;
		done = true;
		int action = learn.chooseBoltzmanAction(state, explorationConstant);
		if(action != (int)state[0]) {
			learn.applyReinforcementToLastAction(-1, state);
			done = false;
		} else  {
			state[0] = (int)(state[0] + 1) % 2;
			learn.applyReinforcementToLastAction(1, state);
		}
		
		action = learn.chooseBoltzmanAction(state, explorationConstant);
		if(action != (int)state[0]) {
			learn.applyReinforcementToLastAction(-1, state);
			done = false;
		} else  {
			state[0] = (int)(state[0] + 1) % 2;
			learn.applyReinforcementToLastAction(1, state);
		}
        
        action = learn.chooseBoltzmanAction(state, explorationConstant);
        if(action != (int)state[0]) {
            learn.applyReinforcementToLastAction(-1, state);
            done = false;
        } else  {
            state[0] = (int)(state[0] + 1) % 2;
            learn.applyReinforcementToLastAction(1, state);
        }
        
        action = learn.chooseBoltzmanAction(state, explorationConstant);
        if(action != (int)state[0]) {
            learn.applyReinforcementToLastAction(-1, state);
            done = false;
        } else  {
            state[0] = (int)(state[0] + 1) % 2;
            learn.applyReinforcementToLastAction(1, state);
        }
        
        action = learn.chooseBoltzmanAction(state, explorationConstant);
        if(action != (int)state[0]) {
            learn.applyReinforcementToLastAction(-1, state);
            done = false;
        } else  {
            state[0] = (int)(state[0] + 1) % 2;
            learn.applyReinforcementToLastAction(1, state);
        }
        
        action = learn.chooseBoltzmanAction(state, explorationConstant);
        if(action != (int)state[0]) {
            learn.applyReinforcementToLastAction(-1, state);
            done = false;
        } else  {
            state[0] = (int)(state[0] + 1) % 2;
            learn.applyReinforcementToLastAction(1, state);
        }
        
        explorationConstant *= 0.8;
	}
    
    std::cout << "Times: " << index << "\n";

	std::cout << "input plz\n";
	std::cin >> state[0];
	while(true) {
		std::cout << "Action taken: " << learn.chooseBestAction(state) << "\n";
		std::cout << "reward plz\n";
		std::cin >> reward;
		std::cout << "new state plz\n";
		std::cin >> state[0];
		learn.applyReinforcementToLastAction(reward, state);
        explorationConstant /= 2;
	}

} 