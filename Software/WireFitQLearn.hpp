#ifndef WIREFITQLEARN_H
#define WIREFITQLEARN_H

#include <vector>

#include "Backpropagation.hpp"
#include "Learner.hpp"

namespace net{
	class NeuralNet;
}

namespace rl {

	class Interpolator;
	struct Wire;

	/* An implementation of QLearn that works with continous state action spaces, 
	 * as detailed in http://users.cecs.anu.edu.au/~rsl/rsl_papers/99ai.kambara.pdf.
	 * A wire fitted interpolator function is used in conjunction with a neural network
	 * to turn descrete action vectors paired with rewards, 
	 * to a continuous function that pair an action vector with a reward.
	 */
	class WireFitQLearn : public Learner {
	public:
		net::NeuralNet *network;
		Interpolator *interpolator;
		net::Backpropagation backprop;
		int numberOfWires, actionDimensions;
		double learningRate, devaluationFactor;
		double scalingFactorToMillis;
		double controlPointsGDErrorTarget, controlPointsGDLearningRate;
		int controlPointsGDMaxIterations;
		int baseOfDimensions;
		State lastState;
		Action minAction, maxAction, lastAction;
		
		/* Initializes a WireFitQLearn object with a model NN (network is copied from this), a net::Backpropagation object (used to train network),
		 * a learning rate (dictates how fast the reward values for actions change), a devaluation factor (dictates how much future rewards are valued),
		 * the dimensions of the action vectors, and how many wires the network outputs.
		 */
		WireFitQLearn(net::NeuralNet *modelNetwork, Interpolator *interpolator_, net::Backpropagation backprop_, double learningRate_, double devaluationFactor_, int actionDimensions_, int numberOfWires_, Action minAction_, Action maxAction_, int baseOfDimensions_);

		WireFitQLearn();
		
		// Initializes a WireFitQLearn object from a file
		WireFitQLearn(std::string filename);
		
		// Outputs the action vector that the model thinks will give the most reward
		Action chooseBestAction(State currentState);
		
		/* Gets an action using the Boltzman softmax probability distribution
		 *
		 * Non-random search heuristic used so that the neural network explores actions despite their reward value. 
		 * The lower the exploration constanstant, the more likely it is to pick the best action for the current state.
		 * The number of wires returned is baseOfDimensions raised to the power of the number of actionDimensions (the size of minAction or maxAction).
		 */
		Action chooseBoltzmanAction(State currentState, double explorationConstant);
		
		/* Given the immediate reward from the last action taken and the new state, 
		 * this function updates the correct value for the longterm reward of the last action taken,
		 * devises new control wires for the interpolator using gradient descent
		 * and trains the network to output the new control wires.
		 */
		void applyReinforcementToLastAction(double reward, State newState);

		// Resets the neural network of object so that a random set of control points are outputted
		void reset();
		
		// Stores the WireFitQLearn object in a file
		void storeWireFitQLearn(std::string filename);

	private:
		// Feeds the state into the network, parses to the output of the network into wire form, and outputs these wires
		std::vector<Wire> getWires(State state);

		/*
		 * Gets the number of wires specified on the interpolator function for the given state between the min and max actions given. 
		 * The number of wires returned is baseOfDimensions raised to the power of the number of actionDimensions (the size of minAction or maxAction).
		 */
		std::vector<Wire> getSetOfWires(const State &state,
										int baseOfDimensions);

		// Given a set of wires converts them to the raw output of the NN
		std::vector<double> getRawOutput(std::vector<Wire> wires);
		
		// Gets the highest reward value possible for a given state
		double highestReward(State state);
		
		// Gets the action with the highest reward value for a given state
		Action bestAction(State state);

		// Gets the q value of an action
		double getQValue(double reward, 
			const State &oldState, 
			const State &newState, 
			const Action &action,
			const std::vector<Wire> &controlWires);
		
		// Using gradient descent, outputs a new set of control wires using a new "correct" wire and the old control wires  
		std::vector<Wire> newControlWires(const Wire &correctWire, std::vector<Wire> controlWires);

		void updateBias(double oldReward, double correctReward);
		
		double biasMovingAverage;
		int rewardIterations;
	};
};

#endif
