#ifndef QLEARN_H
#define QLEARN_H

#include <iostream>
#include <vector>
#include <stdlib.h>

#include "NeuralNet.h"
#include "BackPropagation.h"

namespace net {
	class QLearn {
	public:
		std::vector<NeuralNet *> networks;
		Backpropagation backprop;
		int lastAction, numberOfActions;
		double learningRate, devaluationFactor;
		std::vector<double> lastState;

		/* Initializes a QLearn object with the values of learning rate and devaluationFactor.
		 *
		 * Learning rate is a constant between 0 and 1 that dictates how fast the robot learns from reinforcement.
		 * Devaluation factor is a constant between 0 and 1 that weighs future reward vs immediate reward. 
		 * A value of 0 will make the network only value immediate reward, while a value of 1 will make it consider future reward with the same weight as immediate reward. 
		 */
		QLearn(NeuralNet *modelNetwork, Backpropagation backprop_, double learningRate_, double devaluationFactor_, int numberOfActions_);

		// Gets the action that the network deems most benificial for the currentState
		int chooseBestAction(std::vector<double> currentState);

		/* Gets an action using the Boltzman softmax probability distribution
		 *
		 * Non-random search heuristic used so that the neural network explores actions despite their reward value. 
		 * The lower the exploration constanstant, the more likely it is to pick the best action for the current state.
		 */
		int chooseBoltzmanAction(std::vector<double> currentState, double explorationConstant);

		/* Given the immediate reward from the last action taken and the new state, 
		 * this function updates the correct value for the longterm reward of the lastAction and trains the network in charge of the lastAction to output the corect reward value
		 */
		void applyReinforcementToLastAction(double reward, std::vector<double> newState);
	private:
		// Gets the action with the highest reward for a state and gets that action's reward
		void getBestActionAndReward(std::vector<double> state, int &bestAction, double &bestReward);

		// Returns the reward value of the action with the greatest reward.
		double highestReward(std::vector<double> state);

		// Returns the action with the highest reward
		int bestAction(std::vector<double> state);
	};
};

#endif