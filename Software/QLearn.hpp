#ifndef QLEARN_H
#define QLEARN_H

#include <vector>

#include "Learner.hpp"
#include "Backpropagation.hpp"

namespace net{
	class NeuralNet;
}

namespace rl {

	struct Model {
		net::NeuralNet *network;
		Action action;
		std::vector<std::pair<State, double>> history;

		Model(net::NeuralNet * network_, Action action_) {
			network = network_;
			action = action_;
		}

		Model() {}

		void addToHistory(std::pair<State, double> entry) {
			history.push_back(entry);
			if(history.size() > 5) history.erase(history.begin());
		}
	};

	class QLearn : public Learner {
	public:
		/* Initializes a QLearn object with a model network and the values of learning rate and devaluationFactor.
		 *
		 * The model network is used as a model architecture for the networks that will rate the reward of each action.
		 * Learning rate is a constant between 0 and 1 that dictates how fast the robot learns from reinforcement.
		 * Devaluation factor is a constant between 0 and 1 that weighs future reward vs immediate reward. 
		 * A value of 0 will make the network only value immediate reward, while a value of 1 will make it consider future reward with the same weight as immediate reward. 
		 */
		QLearn(net::NeuralNet *modelNetwork, net::Backpropagation backprop_, double learningRate_, double devaluationFactor_, std::vector<Action> possibleActions_);

		/* Initializes a QLearn object with an vector of networks and the values of learning rate and devaluationFactor.
		*
		* The vector of networks contains the networks that will rate the reward of each action
		* Learning rate is a constant between 0 and 1 that dictates how fast the robot learns from reinforcement.
		* Devaluation factor is a constant between 0 and 1 that weighs future reward vs immediate reward.
		* A value of 0 will make the network only value immediate reward, while a value of 1 will make it consider future reward with the same weight as immediate reward.
		*/
		QLearn(std::vector<Model> models_, net::Backpropagation backprop_, double learningRate_, double devaluationFactor_);

		QLearn();

		// Gets the action that the network deems most benificial for the currentState
		Action chooseBestAction(State currentState);

		/* Gets an action using the Boltzman softmax probability distribution
		 *
		 * Non-random search heuristic used so that the neural network explores actions despite their reward value. 
		 * The lower the exploration constanstant, the more likely it is to pick the best action for the current state.
		 */
		Action chooseBoltzmanAction(State currentState, double explorationConstant);

		/* Given the immediate reward from the last action taken and the new state, 
		 * this function updates the correct value for the longterm reward of the lastAction and trains the network in charge of the lastAction to output the corect reward value
		 */
		void applyReinforcementToLastAction(double reward, State newState);

		void reset();
	private:
		net::Backpropagation backprop;
		std::vector<Model> models;
		double learningRate, devaluationFactor;

		State lastState;
		Model lastModel;

		// Gets the action with the highest reward for a state and gets that action's reward
		std::vector<double> getModelRewards(State state);

		// Returns the reward value of the action with the greatest reward.
		double getHighestReward(State state);
	};
};

#endif
