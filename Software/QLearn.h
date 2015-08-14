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

		/* Gets an output from the network for the inputs specified (the variable state) and reward value of the action with the greatest reward. 
		 *
		 *
		 */
		double maxRewardForState(std::vector<double> state);

		// Gets the action that the network deems most benificial for the inputs specified 
		int getAction(std::vector<double> currentState);

		/*Given the immediate reward from the last action taken and the new inputs given to the network. 
		 * Calculates the correct value for the longterm reward of the lastAction and trains the network accordingly.
		 */
		void applyReinforcement(int reward, std::vector<double> newState);
	};
};

#endif