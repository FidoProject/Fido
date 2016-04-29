#ifndef LEARNER_H
#define LEARNER_H

#include <vector>
#include <iostream>

namespace net{
	class NeuralNet;
}

namespace rl {

	typedef std::vector<double> Action;
	typedef std::vector<double> State;

	/** A reinforcement learning system. */
	class Learner {
	public:
		/**
		 * \brief Gets the action that the network deems most beneficial for the currentState
		 *
		 * \param currentState the state for which to choose the action
		 */
		virtual Action chooseBestAction(State currentState) = 0;

		/**
		 * \brief Gets an action using the Boltzman softmax probability distribution
		 *
		 * A non-random search heuristic is used such that the neural network explores actions despite their reward value.
		 * The lower the exploration constant, the more likely it is to pick the best action for the current state.
		 *
		 * \param currentState the state for which to choose the action
		 * \param explorationConstant the Boltzmann temperature constant, determining "exploration"
		 */
		virtual Action chooseBoltzmanAction(State currentState, double explorationConstant) = 0;

		/**
		 * \brief Apply reinforcement to the last action
		 *
		 * Given the immediate reward from the last action taken and the new state,
		 * this function updates the correct value for the longterm reward of the lastAction and trains the network in charge of the lastAction to output the correct reward value
		 *
		 * \param reward the reward given for the last action taken
		 * \param newState the new state
		 */
		virtual void applyReinforcementToLastAction(double reward, State newState) = 0;

		/** Randomizes the leaner.  */
		virtual void reset() = 0;

		/** Initializes the parameters of an learner from a file. Returns false if the interpolator stored in the file is not the correct type */
		virtual bool initFromStream(std::ifstream *in) = 0;

		/**
		 * \brief Stores a Learner object using specified stream.
		 *
		 * \param output pointer to the output stream which the neural network will be written to
		**/
		virtual void store(std::ofstream *output) = 0;
	};
};

#endif
