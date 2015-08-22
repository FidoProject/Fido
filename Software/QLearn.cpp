#include "QLearn.h"

using namespace net;

QLearn::QLearn(NeuralNet *modelNetwork, Backpropagation backprop_, double learningRate_, double devaluationFactor_, int numberOfActions_) {
	backprop = backprop_;
	learningRate = learningRate_;
	devaluationFactor = devaluationFactor_;
	numberOfActions = numberOfActions_;
	lastAction = -1;

	for(int a = 0; a < numberOfActions; a++) networks.push_back(new net::NeuralNet(modelNetwork));
}

QLearn::QLearn() {

}

int QLearn::chooseBestAction(std::vector<double> currentState) {
	int action = bestAction(currentState);
	lastAction = action;
	lastState = currentState;
	return action;
}

int QLearn::chooseBoltzmanAction(std::vector<double> currentState, double explorationConstant) {
	double determiner = (double)rand() / (double)RAND_MAX;
	std::vector<double> exponentTerms;
	double sumOfExponentTerms = 0;

	for(int a = 0; a < networks.size(); a++) {
		double reward = networks[a]->getOutput(currentState)[0];
		double exponentTerm = exp(reward / explorationConstant);
		exponentTerms.push_back(exponentTerm);
		sumOfExponentTerms += exponentTerm;
	}

	double sumOfProbabilities = 0;
	for(int a = 0; a < networks.size(); a++) {
		sumOfProbabilities += (exponentTerms[a] / sumOfExponentTerms);
		if(sumOfProbabilities >= determiner) {
			lastAction = a;
			lastState = currentState;
			return a;
		}
	}

	throw 1;
}

void QLearn::applyReinforcementToLastAction(double reward, std::vector<double> newState) {
	if(lastAction == -1) return;

	double oldValue = highestReward(lastState);
	double feedback = (reward + (devaluationFactor*highestReward(newState)));
	double targetValueForLastState = ((1 - learningRate) * oldValue) + (learningRate*feedback);

	std::cout << "T val: " << targetValueForLastState << "\n";

	backprop.trainOnData(networks[lastAction], { lastState }, { { targetValueForLastState } });
}

void QLearn::getBestActionAndReward(std::vector<double> state, int &bestAction, double &bestReward) {
	bestAction = 0;
	bestReward = -99999;

	for(int a = 0; a < networks.size(); a++) {
		double reward = networks[a]->getOutput(state)[0];
		std::cout << "State:  " << state[0] << "; Reward for " << a << ": " << reward << "\n";
		if(reward > bestReward) {
			bestAction = a;
			bestReward = reward;
		}
	}
}

double QLearn::highestReward(std::vector<double> state) {
	int bestAction;
	double bestReward;
	getBestActionAndReward(state, bestAction, bestReward);

	return bestReward;
}

int QLearn::bestAction(std::vector<double> state) {
	int bestAction;
	double bestReward;
	getBestActionAndReward(state, bestAction, bestReward);

	return bestAction;
}