#include "../include/FidoControlSystem.h"

#include "../include/NeuralNet.h"
#include "../include/Pruner.h"
#include "../include/LSInterpolator.h"


using namespace rl;

FidoControlSystem::FidoControlSystem(int stateDimensions, int actionDimensions, Action minAction, Action maxAction) : WireFitQLearn(stateDimensions, actionDimensions, 1, 12, 4, minAction, maxAction, 11, new rl::LSInterpolator(), new net::Pruner(), 0.95, 0.4)  { }


void FidoControlSystem::applyReinforcementToLastAction(double reward, State newState) {
  std::vector<Wire> controlWires = getWires(lastState);
	double newRewardForLastAction = getQValue(reward, lastState, newState, lastAction, controlWires);

	histories.push_back(History(lastState, newState, lastAction, reward));
	if(histories.size() > 100) {
		histories.erase(histories.begin());
	}

	Wire correctWire = {lastAction, newRewardForLastAction};
	std::vector<Wire> newContolWires = newControlWires(correctWire, controlWires);

	std::vector< std::vector<double> > input(1, lastState);
	std::vector< std::vector<double> > correctOutput(1, getRawOutput(newContolWires));

	std::vector<History> tempHistories(histories);
	while(input.size() < 4) {
		bool foundOne = false;
		double maxDifference = 0;
		auto bestHistory = tempHistories.begin();

		for(auto history = tempHistories.begin(); history != tempHistories.end(); history++) {
			double difference = 0;
			bool equalToOne = false;
			for(unsigned int b = 0; b < input.size(); b++) {
				double localDifference = 0;
				for(unsigned int c = 0; c < input[b].size(); c++) {
					localDifference += fabs(input[b][c] - history->initialState[c]);
				}
				if(localDifference < 0.2) {
					equalToOne = true;
					break;
				}
				difference += localDifference;
			}
			if(equalToOne) continue;
			if(difference > maxDifference) {
				foundOne = true;
				maxDifference = difference;
				bestHistory = history;
			}
		}

		if(!foundOne) break;

		std::vector<Wire> historyControlWires = getWires(bestHistory->initialState);
		input.push_back(bestHistory->initialState);
		correctOutput.push_back(getRawOutput(newControlWires({bestHistory->action, getQValue(bestHistory->reward, bestHistory->initialState, bestHistory->newState, bestHistory->action, historyControlWires)}, historyControlWires)));

		tempHistories.erase(bestHistory);
	}

	trainer->train(network, input, correctOutput);
}

void FidoControlSystem::reset() {
  network->randomizeWeights();
  histories.clear();
}
