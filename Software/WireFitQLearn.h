#ifndef WIREFITQLEARN_H
#define WIREFITQLEARN_H

#include <iostream>
#include <vector>
#include <stdlib.h>

#include "NeuralNet.h"
#include "BackPropagation.h"

#include <SFML/Graphics.hpp>

namespace net {
    struct Wire {
        std::vector<double> action;
        double reward;
    };

    /* An implementation of QLearn that works with continous state action spaces, 
     * as detailed in http://users.cecs.anu.edu.au/~rsl/rsl_papers/99ai.kambara.pdf.
     * A wire fitted interpolator function is used in conjunction with a neural network
     * to turn descrete action vectors paired with rewards, 
     * to a continuous function that pair an action vector with a reward.
     */
    class WireFitQLearn {
    public:
        NeuralNet *network;
        Backpropagation backprop;
        int numberOfWires, actionDimensions;
        double learningRate, devaluationFactor;
        double scalingFactorToMillis;
        double smoothingFactor, e;
        double gradientDescentErrorTarget, gradientDescentLearningRate;
        int gradientDescentMaxIterations;
        std::vector<double> lastState, lastAction;
        
        /* Initializes a WireFitQLearn object with a model NN (network is copied from this), a Backpropagation object (used to train network),
         * a learning rate (dictates how fast the reward values for actions change), a devaluation factor (dictates how much future rewards are valued),
         * the dimensions of the action vectors, and how many wires the network outputs.
         */
        WireFitQLearn(NeuralNet *modelNetwork, Backpropagation backprop_, double learningRate_, double devaluationFactor_, int actionDimensions_, int numberOfWires_);
        
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
         */
        std::vector<double> chooseBoltzmanAction(std::vector<double> currentState, double explorationConstant);

		// Chooses a random action vector between the minmum and maximum vectors given
		std::vector<double> chooseRandomAction(const std::vector<double> &state, const std::vector<double> &minAction, const std::vector<double> &maxAction);
        
        /* Given the immediate reward from the last action taken and the new state, 
         * this function updates the correct value for the longterm reward of the last action taken,
         * devises new control wires for the interpolator using gradient descent
         * and trains the network to output the new control wires.
         */
        void applyReinforcementToLastAction(double reward, std::vector<double> newState, double elapsedTimeMillis);
        
        // Stores the WireFitQLearn object in a file
        void storeWireFitQLearn(std::string filename);

		// Graphs the interpolator function given a range of actions
		void graphInterpolatorFunction(const std::vector<Wire> &controlWires, double minAction, double maxAction, double targetAction);

		// Feeds the state into the network, parses to the output of the network into wire form, and outputs these wires
		std::vector<Wire> getWires(std::vector<double> state);
    private:
        
        
        // Given a set of wires converts them to the raw output of the NN
        std::vector<double> getRawOutput(std::vector<Wire> wires);
        
        // Gets the highest reward value possible for a given state
        double highestReward(std::vector<double> state);
        
        // Gets the action with the highest reward value for a given state
        std::vector<double> bestAction(std::vector<double> state);
        
        // Using gradient descent, outputs a new set of control wires using a new "correct" wire and the old control wires  
        std::vector<Wire> newControlWires(const Wire &correctWire, std::vector<Wire> controlWires);
        
        // The partial derivative of the wire interpolator function with respect to the reward of a control wire
        double rewardDerivative(const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires);

        // The partial derivative of the wire interpolator function with respect to the value of one term of the action vector of a control wire
        double actionTermDerivative(double actionTerm, double wireActionTerm, const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires);
        
        // Uses the wire interpolator function to compute the reward of an action vector given a set of control wires
        double getRewardUsingInterpolator(const std::vector<Wire> &controlWires, const std::vector<double> &action);
        
        // Computes a weighted distance between the action vector of the wire and an action vector
        double distanceBetweenWireAndAction(const Wire &wire, const std::vector<double> &action, double maxReward);

        // Computes the weighted sum of all the distances between the wires and the action vector 
        double weightedSum(const std::vector<Wire> &wires, const std::vector<double> &action, double maxReward);

        // Used in the wire interpolator function and its derivatives for normalization
        double normalize(const std::vector<Wire> &wires, const std::vector<double> &action, double maxReward);
    };
};

#endif