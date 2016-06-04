#pragma once

#include <vector>

#include "Learner.h"
#include "Trainer.h"

namespace net {
class NeuralNet;
}

namespace rl {

struct Model {
  net::NeuralNet *network;
  Action action;

  Model(net::NeuralNet * network_, Action action_) {
    network = network_;
    action = action_;
  }

  Model() {}
};

/** A Learner that follows the Q-Learning algorithm */
class QLearn : public Learner {
public:
  /** Initializes a QLearn object with a model network and the values of learning rate and devaluationFactor.
   *
   * \param modelNetwork a neural network that is used as a model architecture for the networks that will rate the reward of each action.
   * \param backprop_ the model that will train the neural networks that will rate the reward of each action
   * \param learningRate_ a constant between 0 and 1 that dictates how fast the robot learns from reinforcement.
   * \param devaluationFactor_ a constant between 0 and 1 that weighs future reward vs immediate reward. A value of 0 will make the network only value immediate reward, while a value of 1 will make it consider future reward with the same weight as immediate reward.
   * \param possibleActions_ all of the actions that this object could possibly choose
   */
  QLearn(net::NeuralNet *modelNetwork, net::Trainer *trainer_, double learningRate_, double devaluationFactor_, std::vector<Action> possibleActions_);

  /** Initializes a QLearn object with an vector of networks and the values of learning rate and devaluationFactor.
  *
  * \param models_ he vector of networks contains the networks that will rate the reward of each action
  * \param backprop_ the model that will train the neural networks that will rate the reward of each action
  * \param learningRate_ a constant between 0 and 1 that dictates how fast the robot learns from reinforcement.
  * \param devaluationFactor_ a constant between 0 and 1 that weighs future reward vs immediate reward. A value of 0 will make the network only value immediate reward, while a value of 1 will make it consider future reward with the same weight as immediate reward.
  */
  QLearn(std::vector<Model> models_, net::Trainer *trainer_, double learningRate_, double devaluationFactor_);

  /** Initializes an empty, non-valid Q-learning object. */
  QLearn();

  /** Gets the action that the network deems most benificial for the current state
   *
   * \param currentState a vector of doubles representing the "inputs" or sensor values of the system
   */
  Action chooseBestAction(State currentState);

  /** Gets an action using the Boltzman softmax probability distribution
   *
   * Non-random search heuristic used so that the neural network explores actions despite their reward value. The lower the exploration constanstant, the more likely it is to pick the best action for the current state.
   * \param currentState a vector of doubles representing the "inputs" or sensor values of the system
   * \param explorationConstant a positive floating point number representing the exploration level of the system. Common values range from 0.01 to 1. The higher this number is, the more likely it is that the system will pick worse actions.
   */
  Action chooseBoltzmanAction(State currentState, double explorationConstant);

  /** Updates expected reward values.
   *
   * Given the immediate reward from the last action taken and the new state, this function updates the correct value for the longterm reward of the lastAction and trains the network in charge of the lastAction to output the corect reward value.
   * \param reward the reward value from the last action
   * \param newState the new state (aka. inputs) to the system
   */
  void applyReinforcementToLastAction(double reward, State newState);

  /** Reverts the system to a newely initialized state
   *
   * Reset's the system's model and wipes the system's memory of past actions, states, and rewards.
  */
  void reset();

  /** Stores this model in a stream. */
  void store(std::ofstream *output);
private:
  net::Trainer *trainer;
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
