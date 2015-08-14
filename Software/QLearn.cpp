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

double QLearn::maxRewardForState(std::vector<double> state) {
	int bestAction = 0;
	double bestScore = -99999;
	
	std::vector<double> input = state;
	for(int a = 0; a < numberOfActions; a++) input.push_back(0);

	for(int a = 0; a < numberOfActions; a++) {
		input[state.size() + a] = 1;
		double output = network->getOutput(input)[0];
		if(output > bestScore) {
			bestScore = output;
			bestAction = a;
		}
		input[state.size() + a] = 0;
	}

	return bestScore;
}

int QLearn::getAction(std::vector<double> currentState) {
	int bestAction = 0;
	double bestScore = -99999;

	std::vector<double> input = currentState;
	for(int a = 0; a < numberOfActions; a++) input.push_back(0);

	int startingIndex = currentState.size();

	for(int a = 0; a < numberOfActions; a++) {
		input[startingIndex + a] = 1;
		double output = network->getOutput(input)[0];
		std::cout << "Score " << output << "\n";
		if(output > bestScore) {
			bestScore = output;
			bestAction = a;
		}
		input[startingIndex + a] = 0;
	}

	lastAction = bestAction;
	lastState = currentState;
	return bestAction;
}



void QLearn::applyReinforcement(int reward, std::vector<double> newState) {
	if(lastAction == -1) return;

	std::cout << "component: " << maxRewardForState(newState) << "\n";
	std::cout << "r: " << reward << "\n";
	std::cout << "l: " << learningRate << "\n";
	double targetValueForLastState = (reward + (learningRate*maxRewardForState(newState)))/2;

	std::vector<double> inputs = lastState;
	for(int a = 0; a < numberOfActions; a++) inputs.push_back(0);
	inputs[lastAction + lastState.size()] = 1;
	
	for(int a = 0; a < inputs.size(); a++) std::cout << "input: " << inputs[a] << "\n";

	std::cout << "Target value: " << targetValueForLastState << "\n";
	backprop.trainOnData(network, { inputs }, { { targetValueForLastState } });
}