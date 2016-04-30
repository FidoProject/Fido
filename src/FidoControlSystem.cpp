#include "../include/FidoControlSystem.h"

#include "../include/NeuralNet.h"
#include "../include/Backpropagation.h"
#include "../include/Adadelta.h"
#include "../include/LSInterpolator.h"


using namespace rl;

FidoControlSystem::FidoControlSystem(int stateDimensions, Action minAction, Action maxAction, int baseOfDimensions) : WireFitQLearn(stateDimensions, minAction.size(), 1, 6, 3, minAction, maxAction, baseOfDimensions, new rl::LSInterpolator(), new net::Adadelta(0.95, 0.2, 10000), 1, 0)  {
	explorationLevel = initialExploration;
}


std::vector<double> FidoControlSystem::chooseBoltzmanActionDynamic(State state) {
	// Exploration is equal to what it was set to in applyReinforcementToLastAction
	return WireFitQLearn::chooseBoltzmanAction(state, explorationLevel);
}

void FidoControlSystem::applyReinforcementToLastAction(double reward, State newState) {
	// History sampling
	if(histories.size() > 0) {
		std::vector< std::vector<double> > input;
		std::vector< std::vector<double> > correctOutput;

		std::vector<History> selectedHistories = selectHistories();
		for(History history : selectedHistories) {

			std::vector<Wire> historyControlWires = getWires(history.initialState);
			double newRewardForLastAction = getQValue(history.reward, history.initialState, history.newState, history.action, historyControlWires);

			Wire correctHistoryWire = {history.action, newRewardForLastAction};
			std::vector<Wire> newContolWires = newControlWires(correctHistoryWire, historyControlWires);

			input.push_back(history.newState);
			correctOutput.push_back(getRawOutput(newContolWires));
		}
		trainer->train(network, input, correctOutput);
	}


	// Compute new input and output
	std::vector<Wire> controlWires = getWires(lastState);
	double newRewardForLastAction = getQValue(reward, lastState, newState, lastAction, controlWires);
	Wire correctWire = {lastAction, newRewardForLastAction};
	std::vector<Wire> newContolWires = newControlWires(correctWire, controlWires);

	// Compute new boltzman value using uncertainty value
	double error = getError(lastState, getRawOutput(newContolWires));
	explorationLevel = pow(error, 3) * 60000000;
	if(explorationLevel < 0.00001) {
		explorationLevel = 0.00001;
	}
	if(explorationLevel > 1000) {
		explorationLevel = 1000;
	}
	lastError = error;

	trainer->train(network, {lastState}, {getRawOutput(newContolWires)});

	// Add to histories
	histories.push_back(History(lastState, newState, lastAction, reward));
	if(histories.size() > 200) {
		histories.erase(histories.begin());
	}
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
