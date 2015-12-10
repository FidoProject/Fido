#ifndef WIREFITQLEARN_H
#define WIREFITQLEARN_H

#include <vector>

#include "BackPropagation.h"

namespace net {

	class NeuralNet;
	class Interpolator;
	struct Wire;

    /* An implementation of QLearn that works with continous state action spaces, 
     * as detailed in http://users.cecs.anu.edu.au/~rsl/rsl_papers/99ai.kambara.pdf.
     * A wire fitted interpolator function is used in conjunction with a neural network
     * to turn descrete action vectors paired with rewards, 
     * to a continuous function that pair an action vector with a reward.
     */
    class WireFitQLearn {
    public:
        NeuralNet *network;
        Interpolator *interpolator;
        Backpropagation backprop;
        int numberOfWires, actionDimensions;
        double learningRate, devaluationFactor;
        double scalingFactorToMillis;
        double controlPointsGDErrorTarget, controlPointsGDLearningRate;
        int controlPointsGDMaxIterations;
        std::vector<double> lastState, lastAction;
        
        /* Initializes a WireFitQLearn object with a model NN (network is copied from this), a Backpropagation object (used to train network),
         * a learning rate (dictates how fast the reward values for actions change), a devaluation factor (dictates how much future rewards are valued),
         * the dimensions of the action vectors, and how many wires the network outputs.
         */
        WireFitQLearn(NeuralNet *modelNetwork, Interpolator *interpolator_, Backpropagation backprop_, double learningRate_, double devaluationFactor_, int actionDimensions_, int numberOfWires_);
        
        // Initializes a WireFitQLearn object from a file
        WireFitQLearn(std::string filename);
        
        // Initializes and empty WireFitQLearn object
        WireFitQLearn();
        
        // Outputs the action vector that the model thinks will give the most reward
        std::vector<double> chooseBestAction(std::vector<double> currentState);
        
        /* Gets an action using the Boltzman softmax probability distribution
         *
         * Non-random search heuristic used so that the neural network explores actions despite their reward value. 
         * The lower the exploration constanstant, the more likely it is to pick the best action for the current state.
		 * The number of wires returned is baseOfDimensions raised to the power of the number of actionDimensions (the size of minAction or maxAction).
         */
        std::vector<double> chooseBoltzmanAction(std::vector<double> currentState,
												const std::vector<double> &minAction,
												const std::vector<double> &maxAction,
												int baseOfDimensions,
												double explorationConstant);

		// Chooses a random action vector between the minmum and maximum vectors given
		std::vector<double> chooseRandomAction(const std::vector<double> &state, const std::vector<double> &minAction, const std::vector<double> &maxAction);
        
        /* Given the immediate reward from the last action taken and the new state, 
         * this function updates the correct value for the longterm reward of the last action taken,
         * devises new control wires for the interpolator using gradient descent
         * and trains the network to output the new control wires.
         */
        void applyReinforcementToLastAction(double reward, std::vector<double> newState, double elapsedTimeMillis);

		// Resets the neural network of object so that a random set of control points are outputted
		void resetControlPoints();

		void repeated(const std::vector< std::vector<double> > &actions, 
			const std::vector< std::vector<double> > &oldStates, 
			const std::vector<double> &immediateRewards, 
			const std::vector< std::vector<double> > &newStates,
			const std::vector<double> &elapsedTimes,
			int numberOfIterations);
        
        // Stores the WireFitQLearn object in a file
        void storeWireFitQLearn(std::string filename);

		// Graphs the interpolator function given a range of actions
		void graphInterpolatorFunction(const std::vector<Wire> &controlWires, double minAction, double maxAction, double targetAction);

		// Feeds the state into the network, parses to the output of the network into wire form, and outputs these wires
		std::vector<Wire> getWires(std::vector<double> state);

		/*
		 * Gets the number of wires specified on the interpolator function for the given state between the min and max actions given. 
		 * The number of wires returned is baseOfDimensions raised to the power of the number of actionDimensions (the size of minAction or maxAction).
		 */
		std::vector<Wire> getSetOfWires(const std::vector<double> &state, 
										const std::vector<double> &minAction, 
										const std::vector<double> &maxAction, 
										int baseOfDimensions);
    private:
        // Given a set of wires converts them to the raw output of the NN
        std::vector<double> getRawOutput(std::vector<Wire> wires);
        
        // Gets the highest reward value possible for a given state
        double highestReward(std::vector<double> state);
        
        // Gets the action with the highest reward value for a given state
        std::vector<double> bestAction(std::vector<double> state);

		// Gets the q value of an action
		double getQValue(double reward, 
			const std::vector<double> &oldState, 
			const std::vector<double> &newState, 
			const std::vector<double> &action, 
			double elapsedTimeMillis, 
			const std::vector<Wire> &controlWires);
        
        // Using gradient descent, outputs a new set of control wires using a new "correct" wire and the old control wires  
        std::vector<Wire> newControlWires(const Wire &correctWire, std::vector<Wire> controlWires);

		void updateBias(double oldReward, double correctReward);
		
		double biasMovingAverage;
		int rewardIterations;
	};
};

#endif
