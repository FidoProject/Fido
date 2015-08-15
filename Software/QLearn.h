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
		NeuralNet *network;
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
		QLearn(NeuralNet *network_, Backpropagation backprop_, double learningRate_, double devaluationFactor_, int numberOfActions_);

		// Gets the action that the network deems most benificial for the currentState
		int chooseAction(std::vector<double> currentState);

		/* Given the immediate reward from the last action taken and the new state 
		 * Calculates the correct value for the longterm reward of the lastAction and trains the network accordingly to output this correctly
		 */
		void applyReinforcement(int reward, std::vector<double> newState);
	private:
		std::vector<double> nnInputForStateAndAction(std::vector<double> state, int action);

		void getBestActionAndReward(std::vector<double> state, int &bestAction, double &bestReward);

		// Gets the reward value of the action with the greatest reward.
		double highestReward(std::vector<double> state);

		int bestAction(std::vector<double> state);
	};
};

#endif