#include "../include/WireFitQLearn.h"

#include <assert.h>
#include <random>
#include <iostream>
#include <thread>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <random>
#include <cfloat>

#include "../include/Interpolator.h"
#include "../include/NeuralNet.h"

using namespace rl;

WireFitQLearn::WireFitQLearn(unsigned int stateDimensions, unsigned int actionDimensions_, unsigned int numHiddenLayers, unsigned int numNeuronsPerHiddenLayer, unsigned int numberOfWires_, Action minAction_, Action maxAction_, unsigned int baseOfDimensions_, Interpolator *interpolator_, net::Trainer *trainer_, double learningRate_, double devaluationFactor_) {

  assert(minAction_.size() == actionDimensions_);
  assert(maxAction_.size() == actionDimensions_);

  trainer = trainer_;
  learningRate = learningRate_;
  devaluationFactor = devaluationFactor_;
  actionDimensions = actionDimensions_;
  numberOfWires = numberOfWires_;
  lastAction = std::vector<double>(actionDimensions);
  interpolator = interpolator_;
  minAction = minAction_;
  maxAction = maxAction_;

  modelNet = new net::NeuralNet(stateDimensions, numberOfWires_ * (actionDimensions_+1), numHiddenLayers, numNeuronsPerHiddenLayer, "sigmoid");
  modelNet->setOutputActivationFunction("simpleLinear");

  network = new net::NeuralNet(modelNet);

  controlPointsGDErrorTarget = 0.001;
  controlPointsGDLearningRate = 0.01;
  controlPointsGDMaxIterations = 10000;

  baseOfDimensions = baseOfDimensions_;
}

WireFitQLearn::WireFitQLearn() {

}

WireFitQLearn::WireFitQLearn(std::ifstream *input) {

}

Action WireFitQLearn::chooseBestAction(State currentState) {
  std::vector<double> action = bestAction(currentState);
  lastAction = action;
  lastState = currentState;
  return action;
}

Action WireFitQLearn::chooseBoltzmanAction(State currentState,
    double explorationConstant) {

  if (explorationConstant < 0.01) explorationConstant = 0.01;

  double determiner = (double)rand() / (double)RAND_MAX;
  std::vector<double> exponentTerms;
  double sumOfExponentTerms = 0;
  std::vector<Wire> wires = getSetOfWires(currentState, baseOfDimensions);

  for(unsigned int a = 0; a < wires.size(); a++) {
    double exponentTerm = exp(wires[a].reward / explorationConstant);
    exponentTerms.push_back(exponentTerm);
    sumOfExponentTerms += exponentTerm;
  }

  double sumOfProbabilities = 0;
  for(unsigned int a = 0; a < wires.size(); a++) {
    sumOfProbabilities += (exponentTerms[a] / sumOfExponentTerms);
    if(sumOfProbabilities >= determiner) {
      lastAction = wires[a].action;
      lastState = currentState;
      return wires[a].action;
    }
  }

  /// Incase a floating point error resulted in no wire being chosen
  lastAction = wires[wires.size() - 1].action;
  lastState = currentState;
  return wires[wires.size() - 1].action;

}

void WireFitQLearn::applyReinforcementToLastAction(double reward, State newState) {
  std::vector<Wire> controlWires = getWires(lastState);
  double newRewardForLastAction = getQValue(reward, lastState, newState, lastAction, controlWires);

  Wire correctWire = {lastAction, newRewardForLastAction};
  std::vector<Wire> newContolWires = newControlWires(correctWire, controlWires);

  std::vector< std::vector<double> > input(1, lastState);
  std::vector< std::vector<double> > correctOutput(1, getRawOutput(newContolWires));

  trainer->train(network, input, correctOutput);
}

void WireFitQLearn::reset() {
  network = new net::NeuralNet(modelNet);
  network->randomizeWeights();
  std::cout << "number: " << network->numberOfHiddenNeurons() << "\n";
}

void WireFitQLearn::store(std::ofstream *output) {
  assert(output->is_open());
  *output << learningRate << " " << devaluationFactor << "\n";
  *output << actionDimensions << " " << numberOfWires << "\n";
  *output << controlPointsGDErrorTarget << " " << controlPointsGDLearningRate << " " << controlPointsGDMaxIterations <<  " " << baseOfDimensions << "\n";

  for(unsigned int a = 0; a < minAction.size(); a++) {
    *output << minAction[a] << " ";
  }
  for(unsigned int a = 0; a < maxAction.size(); a++) {
    *output << maxAction[a] << " ";
  }
  for(unsigned int a = 0; a < lastAction.size(); a++) {
    *output << lastAction[a] << " ";
  }

  trainer->store(output);
  interpolator->store(output);
  network->store(output);

  output->close();
}

std::vector<Wire> WireFitQLearn::getWires(State state) {
  std::vector<Wire> wires(numberOfWires);
  std::vector<double> rawOutput = network->getOutput(state);

  for(unsigned int a = 0; a < numberOfWires; a++) {
    int currentIndex = a * (actionDimensions + 1);
    Wire wire;
    wire.action = std::vector<double>(actionDimensions);
    for(unsigned int b = 0; b < actionDimensions; b++) {
      wire.action[b] = rawOutput[currentIndex + b];
    }

    wire.reward = rawOutput[currentIndex + actionDimensions];

    wires[a] = wire;
  }

  return wires;
}

std::vector<Wire> WireFitQLearn::getSetOfWires(const State &state, int baseOfDimensions) {

  assert(baseOfDimensions > 1);

  std::vector<Wire> controlWires = getWires(state);

  std::vector<double> scaleVector(minAction.size());
  for(unsigned int a = 0; a < minAction.size(); a++) {
    scaleVector[a] = (maxAction[a] - minAction[a]) / ((double)baseOfDimensions - 1);
  }

  int numberOfWiresReturned = pow(baseOfDimensions, actionDimensions);
  std::vector<Wire> wires(numberOfWiresReturned);

  std::vector<int> iteratorVector(actionDimensions);
  for(unsigned int a = 0; a < iteratorVector.size(); a++) {
    iteratorVector[a] = 0;
  }

  for(int a = 0; a < numberOfWiresReturned; a++) {
    Wire wire;
    for(unsigned int actionDimension = 0; actionDimension < minAction.size(); actionDimension++) {
      wire.action.push_back(iteratorVector[actionDimension] * scaleVector[actionDimension] + minAction[actionDimension]);
    }
    wire.reward = interpolator->getReward(controlWires, wire.action);
    wires[a] = (wire);

    /// Increment iterator vector
    iteratorVector[iteratorVector.size() - 1]++;
    for(int iteratorVectorIndex = iteratorVector.size() - 1; iteratorVectorIndex >= 0; iteratorVectorIndex--) {
      if (iteratorVector[iteratorVectorIndex] >= baseOfDimensions) {
        iteratorVector[iteratorVectorIndex] = 0;
        if(iteratorVectorIndex > 0)iteratorVector[iteratorVectorIndex - 1]++;
      }
    }
  }

  return wires;
}

std::vector<double> WireFitQLearn::getRawOutput(std::vector<Wire> wires) {
  std::vector<double> rawOutput(wires.size() * (actionDimensions + 1));

  for(unsigned int a = 0; a < wires.size(); a++) {
    int currentIndex = a * (actionDimensions + 1);
    Wire wire = wires[a];
    for(unsigned int b = 0; b < wire.action.size(); b++) {
      rawOutput[currentIndex + b] = wire.action[b];
    }

    rawOutput[currentIndex + wire.action.size()] = wire.reward;
  }

  return rawOutput;
}


double WireFitQLearn::highestReward(State state) {
  std::vector<Wire> wires = getWires(state);
  double bestReward = -99999;

  for(unsigned int a = 0; a < wires.size(); a++) {
    if(wires[a].reward > bestReward) bestReward = wires[a].reward;
  }

  return bestReward;
}

Action WireFitQLearn::bestAction(State state) {
  std::vector<Wire> wires = getWires(state);
  double bestReward = DBL_MIN;
  std::vector<double> *bestAction = &wires[0].action;

  for(unsigned int a = 0; a < wires.size(); a++) {
    if(wires[a].reward > bestReward) {
      bestAction = &wires[a].action;
      bestReward = wires[a].reward;
    }
  }

  return *bestAction;
}

double WireFitQLearn::getQValue(double reward,
                                const State &oldState,
                                const State &newState,
                                const Action &action,
                                const std::vector<Wire> &controlWires) {
  /// Update Q value according to adaptive learning
  double oldRewardForLastAction = interpolator->getReward(controlWires, action);
  double feedback = reward + devaluationFactor * highestReward(newState);
  double qValue = ((1 - learningRate) * oldRewardForLastAction) + (learningRate*feedback);

  return qValue;
}

std::vector<Wire> WireFitQLearn::newControlWires(const Wire &correctWire, std::vector<Wire> controlWires) {
  double error = 0;
  int iterations = 0;

  do {
    for(unsigned int a = 0; a < controlWires.size(); a++) {
      double deltaReward = -2 * (-interpolator->getReward(controlWires, correctWire.action) + correctWire.reward)*interpolator->rewardDerivative(correctWire.action, controlWires[a], controlWires);
      controlWires[a].reward = controlWires[a].reward - controlPointsGDLearningRate*deltaReward;
      for(unsigned int b = 0; b < controlWires[a].action.size(); b++) {
        double deltaActionTerm = -2 * (-interpolator->getReward(controlWires, correctWire.action) + correctWire.reward)*interpolator->actionTermDerivative(correctWire.action[b], controlWires[a].action[b], correctWire.action, controlWires[a], controlWires);
        controlWires[a].action[b] = controlWires[a].action[b] - controlPointsGDLearningRate*deltaActionTerm;
      }
    }

    error = pow(correctWire.reward - interpolator->getReward(controlWires, correctWire.action), 2);
    iterations++;
  } while(error > controlPointsGDErrorTarget && iterations < controlPointsGDMaxIterations);

  return controlWires;
}

std::vector<Wire> WireFitQLearn::newControlWires(const std::vector<Wire> &correctWires, std::vector<Wire> controlWires) {
  double error = 0;
  int iterations = 0;

  do {
    for(const auto &correctWire : correctWires) {
      for(unsigned int a = 0; a < controlWires.size(); a++) {
        double deltaReward = -2 * (-interpolator->getReward(controlWires, correctWire.action) + correctWire.reward)*interpolator->rewardDerivative(correctWire.action, controlWires[a], controlWires);
        controlWires[a].reward = controlWires[a].reward - controlPointsGDLearningRate*deltaReward;
        for(unsigned int b = 0; b < controlWires[a].action.size(); b++) {
          double deltaActionTerm = -2 * (-interpolator->getReward(controlWires, correctWire.action) + correctWire.reward)*interpolator->actionTermDerivative(correctWire.action[b], controlWires[a].action[b], correctWire.action, controlWires[a], controlWires);
          controlWires[a].action[b] = controlWires[a].action[b] - controlPointsGDLearningRate*deltaActionTerm;
        }
      }
    }

    error = 0;
    for(const auto &correctWire : correctWires) {
      error += pow(correctWire.reward - interpolator->getReward(controlWires, correctWire.action), 2);
    }

    //std::cout << "Error: " << error << "\n";
    iterations++;
  } while(error > controlPointsGDErrorTarget*correctWires.size()*20 && iterations < controlPointsGDMaxIterations);
  if(iterations == controlPointsGDMaxIterations) std::cout << "Hit max iterwith interpolator. Error: " << error << "\n";
  return controlWires;
}
