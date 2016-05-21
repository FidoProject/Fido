#include "../include/FidoControlSystem.h"

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

	std::cout << "----------RESIZING-----------\n";
	if(selectedHistories.size() > 3) {
		bool didChange = false;
		net::Pruner pruner;
		net::NeuralNet originalNet;
		int iter = 0;
		while(iter < 1) {
			iter++;
			originalNet = net::NeuralNet(network);

			// Randomize
			for(net::Layer &l : network->net) {
				for(net::Neuron &n : l.neurons) {
					n.randomizeWeights();
				}
			}

			double totalCurrentError = trainOnHistories(selectedHistories, 0.001, 1);

			if(network->numberOfHiddenNeurons() > 4) {
				pruner.pruneRandomnly(network);
				double totalPrunedError = trainOnHistories(selectedHistories, 0.001, 1);
				if(totalPrunedError*1.05 < totalCurrentError) {
					didChange = true;
					continue;
				}
			}

			*network = originalNet;
			network->net[0].neurons.push_back(net::Neuron(network->numberOfInputs()));
			for(net::Neuron &n : network->net[1].neurons) {
				n.weights.push_back(1);
				n.randomizeWeights();
			}
			double totalAddedError = trainOnHistories(selectedHistories, 0.001, 1);
			if(totalAddedError*1.05 < totalCurrentError) {
				didChange = true;
				continue;
			}

			*network = originalNet;
			break;
		}

		if(didChange == false) {
			*network = originalNet;
		}
		std::cout << "Num neurons: " << network->numberOfHiddenNeurons() << "\n";
	}
	std::cout << "-----------------\n";

	trainOnHistories(selectedHistories, 0.02, 100);
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

double FidoControlSystem::trainOnHistories(std::vector<FidoControlSystem::History> selectedHistories, double allowedError, unsigned int maxIterations) {

	double totalError = DBL_MAX;
	net::Adadelta tempTrainer = net::Adadelta(0.95, allowedError, 100);
	unsigned int iter = 0;
	do {
		std::vector< std::vector<double> > input, correctOutput;
		for(History history : selectedHistories) {
			std::vector<Wire> historyControlWires = getWires(history.initialState);
			double newRewardForLastAction = getQValue(history.reward, history.initialState, history.newState, history.action, historyControlWires);

			Wire correctHistoryWire = {history.action, newRewardForLastAction};
			std::vector<Wire> newContolWires = newControlWires(correctHistoryWire, historyControlWires);

			input.push_back(history.initialState);
			correctOutput.push_back(getRawOutput(newContolWires));

			tempTrainer.train(network, {input.back()}, {correctOutput.back()});
		}

		totalError = 0;
		for(unsigned int a = 0; a < input.size(); a++) {
			std::vector<Wire> wires = getWires(input[a]);
			for(unsigned int b = 0; b < input[a].size(); b++) {
				totalError += pow(selectedHistories[a].reward - interpolator->getReward(wires, selectedHistories[a].action), 2);
			}
		}

		std::cout << "Error: " << totalError << "\n";
		iter++;
	} while (totalError > allowedError*selectedHistories.size() && iter < maxIterations);

	return totalError;
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
	return histories;
}

std::vector<Wire> FidoControlSystem::newControlWiresForHistory(History history) {
	std::vector<Wire> controlWires = getWires(history.initialState);
	double newRewardForLastAction = getQValue(history.reward, history.initialState, history.newState, history.action, controlWires);
	Wire correctWire = {history.action, newRewardForLastAction};
	return WireFitQLearn::newControlWires(correctWire, controlWires);
}
