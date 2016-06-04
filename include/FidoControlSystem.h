#pragma once

#include "WireFitQLearn.h"

namespace rl {

/** A highly effective reinforcement learning control system (Truell and Gruenstein) */
class FidoControlSystem : public WireFitQLearn {
public:
  /** Initializes a FidoControlSystem
   *
   * \param stateDimensions the number of dimensions of the state being fed to the control system (aka. number of elements in the state vector)
   * \param minAction the minimum possible action (e.g. a vector of doubles) that the control system may output
   * \param maxAction the maximum possible action (e.g. a vector of doubles) that the control system may output
   * \param baseOfDimensions the number of possible descrete values in each dimension. Ex. if baseOfDimensions=2, minAction={0, 0}, maxAction={1, 1}, possibleActions={{0, 0}, {0, 1}, {1, 0}, {1, 1}}.
   */
  FidoControlSystem(int stateDimensions, Action minAction, Action maxAction, int baseOfDimensions);

  std::vector<double> chooseBoltzmanActionDynamic(State state);

  /** Update the control system's model, by giving it reward for its last action.
   *
   * \param reward the reward associated with the control system's last action
   * \param newState the new state vector (needed because states may change after performing an action)
  */
  void applyReinforcementToLastAction(double reward, State newState);

  /** Reverts the control system to a newely initialized state
   *
   * Reset's the control system's model and wipes the system's memory of past actions, states, and rewards.
  */
  void reset();

  const double initialExploration = 1;
  const unsigned int samplesOfHistory = 100;
  double explorationLevel;
  double lastUncertainty;

protected:

  struct History {
    State initialState, newState;
    Action action;
    double reward;

    History(State initialState_, State newState_, Action action_, double reward_) {
      initialState = initialState_;
      newState = newState_;
      action = action_;
      reward = reward_;
    }

    bool operator==(const History& other) {
      return initialState == other.initialState && newState == other.newState && action == other.action && reward == other.reward;
    }
  };

  std::vector<History> histories;

  std::vector<FidoControlSystem::History> selectHistories();
  double trainOnHistories(std::vector<FidoControlSystem::History> selectedHistories, double allowedError, unsigned int maxIterations);
  void adjustExploration(double uncertainty);
  double getError(std::vector<double> input, std::vector<double> correctOutput);
  std::vector<Wire> newControlWiresForHistory(History history);
};
};
