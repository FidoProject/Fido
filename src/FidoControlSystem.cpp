#include "../include/FidoControlSystem.h"

#include "../include/NeuralNet.h"
#include "../include/Backpropagation.h"
#include "../include/LSInterpolator.h"


using namespace rl;

FidoControlSystem::FidoControlSystem(int stateDimensions, Action minAction, Action maxAction, int baseOfDimensions) : WireFitQLearn(stateDimensions, minAction.size(), 1, 20, 3, minAction, maxAction, baseOfDimensions, new rl::LSInterpolator(), new net::Backpropagation(), 1, 0)  {
	explorationLevel = initialExploration;
}


std::vector<double> FidoControlSystem::chooseBoltzmanActionDynamic(State state) {
	// Exploration is equal to what it was set to in applyReinforcementToLastAction
	return WireFitQLearn::chooseBoltzmanAction(state, explorationLevel);
}

void FidoControlSystem::applyReinforcementToLastAction(double reward, State newState) {
	std::vector<Wire> controlWires = getWires(lastState);
	double newRewardForLastAction = getQValue(reward, lastState, newState, lastAction, controlWires);

	histories.push_back(History(lastState, newState, lastAction, reward));
	if(histories.size() > 200) {
		histories.erase(histories.begin());
	}

	Wire correctWire = {lastAction, newRewardForLastAction};
	std::vector<Wire> newContolWires = newControlWires(correctWire, controlWires);

	std::vector< std::vector<double> > input(1, lastState);
	std::vector< std::vector<double> > correctOutput(1, getRawOutput(newContolWires));

	// Select the last (most recent) 20 histories as input
	std::vector<History> selectedHistories = selectHistories();
	for(History history : selectedHistories) {

		std::vector<Wire> historyControlWires = getWires(history.initialState);
		double newRewardForLastAction = getQValue(history.reward, history.initialState, history.newState, history.action, historyControlWires);

		Wire correctHistoryWire = {history.action, newRewardForLastAction};
		std::vector<Wire> newContolWires = newControlWires(correctHistoryWire, historyControlWires);

		input.push_back(history.newState);
		correctOutput.push_back(getRawOutput(newContolWires));
	}


	// Play back all of the histories
	for(int inputIndex = input.size()-1; inputIndex > 0; inputIndex--) {
		trainer->train(network, {input[inputIndex]}, {correctOutput[inputIndex]});
	}

	//std::cout << getError(input[0], correctOutput[0]) << ", ";
	double error = getError(input[0], correctOutput[0]);
	if(input.size() == samplesOfHistory/2.0) {
		//explorationLevel = pow(error, 3) * 60000000;
		explorationLevel = 0.02 * pow(error, 3) / pow(lastError, 3);
	} else if(input.size() >= samplesOfHistory/2.0) {
 		explorationLevel *= pow(error, 3) / pow(lastError, 3);
	}
	if(explorationLevel < 0.00001) {
		explorationLevel = 0.00001;
	}
	if(explorationLevel > 1000) {
		explorationLevel = 1000;
	}
	lastError = error;
	//std::cout << explorationLevel << ", " << reward << "\n";

	trainer->train(network, {input[0]}, {correctOutput[0]});
}

void FidoControlSystem::reset() {
	std::cout << "RESET\n";
	WireFitQLearn::reset();
	histories.clear();
	explorationLevel = initialExploration;
}


double FidoControlSystem::getError(std::vector<double> input, std::vector<double> correctOutput) {
	double totalError = 0;
	std::vector<double> output = network->getOutput(input);
	for(unsigned int a = 0; a < output.size(); a++) {
		totalError += pow(output[a] - correctOutput[a], 2);
	}

	return totalError / (double)output.size();
}

std::vector<FidoControlSystem::History> FidoControlSystem::selectHistories() {
	std::vector<History> selectedHistories;
	std::vector<History> tempHistories(histories);
	while(selectedHistories.size() < samplesOfHistory && tempHistories.size() > 0) {
		auto bestHistory = tempHistories.end()-1;

		selectedHistories.push_back(*bestHistory);
		tempHistories.erase(bestHistory);
	}

	return selectedHistories;
}
