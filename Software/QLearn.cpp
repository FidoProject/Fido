#include "QLearn.h"

using namespace net;

QLearn::QLearn(NeuralNet *network_, Backpropagation backprop_, double learningRate_, double devaluationFactor_, int numberOfActions_) {
	network = network_;
	backprop = backprop_;
	learningRate = learningRate_;
	devaluationFactor = devaluationFactor_;
	numberOfActions = numberOfActions_;
	lastAction = -1;
}

int QLearn::chooseAction(std::vector<double> currentState) {
	int action = bestAction(currentState);
	lastAction = action;
	lastState = currentState;
	return action;
}

void QLearn::applyReinforcement(int reward, std::vector<double> newState) {
	if(lastAction == -1) return;

	double targetValueForLastState = (reward + (learningRate*highestReward(newState))) / 2;
	std::vector<double> nnInput = nnInputForStateAndAction(lastState, lastAction);
	backprop.trainOnData(network, { nnInput }, { { targetValueForLastState } });
}

std::vector<double> QLearn::nnInputForStateAndAction(std::vector<double> state, int action) {
	std::vector<double> nnInput = state;
	for(int a = 0; a < numberOfActions; a++) nnInput.push_back(0);
	nnInput[state.size() + action] = 1;

	return nnInput;
}

void QLearn::getBestActionAndReward(std::vector<double> state, int &bestAction, double &bestReward) {
	bestAction = 0;
	bestReward = -99999;

	std::vector<double> nnInput = state;
	for(int a = 0; a < numberOfActions; a++) nnInput.push_back(0);

	for(int a = 0; a < numberOfActions; a++) {
		nnInput[state.size() + a] = 1;
		double rewardForAction = network->getOutput(nnInput)[0];
		if(rewardForAction > bestReward) {
			bestReward = rewardForAction;
			bestAction = a;
		}
		nnInput[state.size() + a] = 0;
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

	return bestReward;
}