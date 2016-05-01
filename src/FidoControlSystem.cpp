#include "../include/FidoControlSystem.h"

#include <cassert>
#include <float.h>
#include <algorithm>

#include "../include/NeuralNet.h"
#include "../include/Backpropagation.h"
#include "../include/Adadelta.h"
#include "../include/Pruner.h"
#include "../include/LSInterpolator.h"

using namespace rl;

FidoControlSystem::FidoControlSystem(int stateDimensions, Action minAction, Action maxAction, int baseOfDimensions) : WireFitQLearn(stateDimensions, minAction.size(), 1, 12, 6, minAction, maxAction, baseOfDimensions, new rl::LSInterpolator(), new net::Adadelta(0.95, 0.15, 10000), 1, 0)  {
	explorationLevel = initialExploration;
}


std::vector<double> FidoControlSystem::chooseBoltzmanActionDynamic(State state) {
	return WireFitQLearn::chooseBoltzmanAction(state, explorationLevel);
}

void FidoControlSystem::applyReinforcementToLastAction(double reward, State newState) {
	// SGD on current scenario
	std::vector<Wire> newContolWires = newControlWiresForHistory(History(lastState, newState, lastAction, reward));

	// Calculate uncertainty and adjust exploration
	double uncertainty = getError(lastState, getRawOutput(newContolWires));
	adjustExploration(uncertainty);
	lastUncertainty = uncertainty;

	// History sample and resize nn
	histories.push_back(History(lastState, newState, lastAction, reward));
	std::vector<History> selectedHistories = selectHistories();
	trainOnHistories(selectedHistories);
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

void FidoControlSystem::trainOnHistories(std::vector<FidoControlSystem::History> selectedHistories) {

	double totalError = DBL_MAX;
	net::Adadelta tempTrainer = net::Adadelta(0.95, 0.01, 100);
	do {
		std::vector< std::vector<double> > input, correctOutput;
		for(History history : selectedHistories) {
			std::vector<Wire> historyControlWires = getWires(history.initialState);
			double newRewardForLastAction = getQValue(history.reward, history.initialState, history.newState, history.action, historyControlWires);

			Wire correctHistoryWire = {history.action, newRewardForLastAction};
			std::vector<Wire> newContolWires = newControlWires(correctHistoryWire, historyControlWires);

			input.push_back(history.newState);
			correctOutput.push_back(getRawOutput(newContolWires));

			tempTrainer.train(network, {input.back()}, {correctOutput.back()});
		}

		totalError = 0;
		for(unsigned int a = 0; a < input.size(); a++) {
			std::vector<double> output = network->getOutput(input[a]);
			for(unsigned int b = 0; b < input[a].size(); b++) {
				totalError += pow(output[b] - correctOutput[a][b], 2);
			}
		}

		std::cout << "Error: " << totalError << "\n";
	} while (totalError > 0.01*selectedHistories.size());

	std::cout << "Num neurons: " << network->numberOfHiddenNeurons() << "\n";
}

void FidoControlSystem::adjustExploration(double uncertainty) {
	explorationLevel = pow(uncertainty, 2) * 10000;
	if(explorationLevel < 0.00001) {
		explorationLevel = 0.00001;
	}
	if(explorationLevel > 1000) {
		explorationLevel = 1000;
	}
	std::cout << "Exploration level: " << explorationLevel << "\n";
}

std::vector<FidoControlSystem::History> FidoControlSystem::selectHistories() {
	std::vector<History> selectedHistories;
	std::vector<History> tempHistories(histories);
	while(selectedHistories.size() < samplesOfHistory && tempHistories.size() > 0) {
		History bestHistory = *(tempHistories.end()-1);
		double maxDifference = DBL_MIN;
		for(History prospectiveHistory : tempHistories) {
			double difference = 0;
			for(History selectedHistory : selectedHistories) {
				for(unsigned int stateIndex = 0; stateIndex < selectedHistory.initialState.size(); stateIndex++) {
					difference += pow(prospectiveHistory.initialState[stateIndex] - selectedHistory.initialState[stateIndex], 2);
				}
			}

			if(difference > maxDifference) {
				maxDifference = difference;
				bestHistory = prospectiveHistory;
			}
		}

		selectedHistories.push_back(bestHistory);
		tempHistories.erase(std::remove(tempHistories.begin(), tempHistories.end(), bestHistory), tempHistories.end());
	}

	return selectedHistories;
}

std::vector<Wire> FidoControlSystem::newControlWiresForHistory(History history) {
	std::vector<Wire> controlWires = getWires(history.initialState);
	double newRewardForLastAction = getQValue(history.reward, history.initialState, history.newState, history.action, controlWires);
	Wire correctWire = {history.action, newRewardForLastAction};
	return WireFitQLearn::newControlWires(correctWire, controlWires);
}

void FidoControlSystem::inputOutputForHistories(std::vector<FidoControlSystem::History> histories, std::vector< std::vector<double> > * input, std::vector< std::vector<double> > * correctOutput) {
	std::map<State, std::vector<History>> historiesByState;

	for(const History &history : histories) {
		bool foundState = false;
		for(const auto &historiesStatePair : historiesByState) {
			if(historiesStatePair.first == history.initialState) {
				foundState = true;
				break;
			}
		}
		if(foundState == false) {
			historiesByState[history.initialState] = {history};
		} else {
			historiesByState[history.initialState].push_back(history);
		}
	}

	for(const auto &historiesStatePair : historiesByState) {
		std::vector<Wire> correctWires;
		for(History history : historiesStatePair.second) {
			std::vector<Wire> controlWires = getWires(history.initialState);
			double newRewardForLastAction = getQValue(history.reward, history.initialState, history.newState, history.action, controlWires);
			correctWires.push_back({history.action, newRewardForLastAction});
		}

		input->push_back(historiesStatePair.first);
		correctOutput->push_back(getRawOutput(WireFitQLearn::newControlWires(correctWires, getWires(historiesStatePair.first))));
	}
}
