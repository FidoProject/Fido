#ifndef LEARNER_H
#define LEARNER_H

#include <vector>

namespace net{
	class NeuralNet;
}

namespace rl {

	typedef std::vector<double> Action;
	typedef std::vector<double> State;

	class Learner {
	public:
		// Gets the action that the network deems most benificial for the currentState
		virtual Action chooseBestAction(State currentState) = 0;

		/* Gets an action using the Boltzman softmax probability distribution
		 *
		 * Non-random search heuristic used so that the neural network explores actions despite their reward value. 
		 * The lower the exploration constanstant, the more likely it is to pick the best action for the current state.
		 */
		virtual Action chooseBoltzmanAction(State currentState, double explorationConstant) = 0;

		/* Given the immediate reward from the last action taken and the new state, 
		 * this function updates the correct value for the longterm reward of the lastAction and trains the network in charge of the lastAction to output the corect reward value
		 */
		virtual void applyReinforcementToLastAction(double reward, State newState) = 0;

		virtual void reset() = 0;
	};
};

#endif
