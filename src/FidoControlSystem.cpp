#include "../include/FidoControlSystem.h"

#include "../include/NeuralNet.h"
#include "../include/Pruner.h"
#include "../include/LSInterpolator.h"


using namespace rl;

FidoControlSystem::FidoControlSystem(int stateDimensions, Action minAction, Action maxAction, int baseOfDimensions) : WireFitQLearn(stateDimensions, minAction.size(), 1, 12, 5, minAction, maxAction, baseOfDimensions, new rl::LSInterpolator(), new net::Backpropagation(), 1, 0)  { }


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

	std::vector<History> tempHistories(histories);
	while(input.size() < 20 && tempHistories.size() > 0) {
		auto bestHistory = tempHistories.begin();

		std::vector<Wire> historyControlWires = getWires(bestHistory->initialState);
		double newRewardForLastAction = getQValue(bestHistory->reward, bestHistory->initialState, bestHistory->newState, bestHistory->action, historyControlWires);

		Wire correctHistoryWire = {bestHistory->action, newRewardForLastAction};
		std::vector<Wire> newContolWires = newControlWires(correctHistoryWire, historyControlWires);

		input.push_back(bestHistory->newState);
		correctOutput.push_back(getRawOutput(newContolWires));

		tempHistories.erase(bestHistory);
	}

	for(int inputIndex = input.size()-1; inputIndex > -1; inputIndex--) {
		trainer->train(network, {input[inputIndex]}, {correctOutput[inputIndex]});
	}
}

void FidoControlSystem::reset() {
	network->randomizeWeights();
	histories.clear();
}
