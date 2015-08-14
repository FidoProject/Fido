#include "QLearn.h"

using namespace net;

QLearn::QLearn(NeuralNet *network_, Backpropogation backprop_, double learningRate_, double devaluationFactor_) {
	network = network_;
	backprop = backprop_;
	lastAction = -1;
}

int QLearn::maxRewardForState(std::vector<double> state) {
	std::vector<double> output = network->getOutput(state);

	int bestAction = 0;
	int bestScore = -99999;
	for(int a = 0; a < output.size(); a++) {
		if(bestScore < output[a]) {
			bestAction = a;
			bestScore = output[a];
		}
	}

	return bestAction;
}

int QLearn::getAction(std::vector<double> currentState) {
	int bestAction = maxRewardForState(currentState);

	lastAction = bestAction;
	lastState = currentState;
	return bestAction;
}



void QLearn::applyReinforcement(int reward, std::vector<double> newState) {
	if(lastAction = -1) return;

	double targetValueForLastState = reward + (learningRate*maxRewardForState(newState));
	backprop.trainOnData(network, { lastState }, { { targetValueForLastState } });
}