#ifndef LEARNER_H
#define LEARNER_H

#include <vector>

namespace net {
	
	class NeuralNet;

	class Learner {
	public:

		// Gets the action that the network deems most benificial for the currentState
		virtual unsigned int chooseBestAction(std::vector<double> currentState) = 0;

		/* Gets an action using the Boltzman softmax probability distribution
		 *
		 * Non-random search heuristic used so that the neural network explores actions despite their reward value. 
		 * The lower the exploration constanstant, the more likely it is to pick the best action for the current state.
		 */
		virtual unsigned int chooseBoltzmanAction(std::vector<double> currentState, double explorationConstant) = 0;

		/* Given the immediate reward from the last action taken and the new state, 
		 * this function updates the correct value for the longterm reward of the lastAction and trains the network in charge of the lastAction to output the corect reward value
		 */
		virtual void applyReinforcementToLastAction(double reward, std::vector<double> newState) = 0;
	};
};

#endif
