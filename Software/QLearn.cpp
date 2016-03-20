#include "QLearn.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <algorithm>

#include "NeuralNet.h"

using namespace rl;

QLearn::QLearn(net::NeuralNet *modelNetwork, net::Backpropagation backprop_, double learningRate_, double devaluationFactor_, std::vector<Action> possibleActions_) {
	backprop = backprop_;
	learningRate = learningRate_;
	devaluationFactor = devaluationFactor_;

	models = std::vector<Model>(possibleActions_.size());
	for(int a = 0; a < possibleActions_.size(); a++) models.push_back(Model(new net::NeuralNet(modelNetwork), possibleActions_[a]));
}

QLearn::QLearn(std::vector< std::pair< net::NeuralNet *, std::vector<double> > > models_, net::Backpropagation backprop_, double learningRate_, double devaluationFactor_, std::vector<double> > possibleActions_) {
	models = models_;
	backprop = backprop_;
	learningRate = learningRate_;
	devaluationFactor = devaluationFactor_;

}

QLearn::QLearn(std::string filename) {
	std::ifstream input;
	input.open(filename.c_str(), std::ifstream::in);
	if(input.is_open()) {
		input >> learningRate >> devaluationFactor >> numberOfActions;
		backprop = net::Backpropagation(&input);
		for(int a = 0; a < numberOfActions; a++) networks.push_back(new net::NeuralNet(&input));

		input.close();
	} else {
		std::cout << "Could not retrieve qlearn from file\n";
		throw 1;
	}
}

Action QLearn::chooseBestAction(State currentState) {
	lastState = currentState;

	std::vector<double> rewards = getModelRewards(currentState);
	lastModel = models[std::max_element(rewards.begin(), rewards.end()) - rewards.bein()];
	return lastModel.action;
}

Action QLearn::chooseBoltzmanAction(State currentState, double explorationConstant) {
	if(explorationConstant < 0.01) {
		std::cout << "Exploration constant (" << explorationConstant << ") is below 0.01. This is too low!. Will cause integer over flow. Assuming an explorationConstant of 0.01 instead.";
		explorationConstant = 0.01;
	}

	double determiner = (double)rand() / (double)RAND_MAX;

	std::vector<double> rewards = getModelRewards(currentState);

	std::vector<double> exponentTerms(models.size());
	double sumOfExponentTerms = 0;
	std::for_each(rewards.begin(), rewards.end(), [](double reward){
		double exponentTerm = exp(reward / explorationConstant);
		exponentTerms.push_back(exponentTerm);
		sumOfExponentTerms += exponentTerm;
	});

	double sumOfProbabilities = 0;
	for(int a = 0; a < exponentTerms.size(); a++) {
		sumOfProbabilities += (exponentTerms[a] / sumOfExponentTerms);
		if(sumOfProbabilities >= determiner) {
			lastModel = models[a];
			lastState = currentState;
			return lastModel.action;
		}
	}

	/// Incase a floating point error resulted in no action
	std::cout << "Floating point error when choosing an action using a Boltzmann selection policy! Choosing last action.";

	lastModel = models[networks.size() - 1];
	lastState = currentState;
	return lastModel.action;
}

void QLearn::applyReinforcementToLastAction(double reward, State newState) {
	if(lastAction == -1) return;
	
	double lr = lastModel.network->getOutput(lastState)[0];
	double targetValueForLastState = lr + learningRate*(reward+(devaluationFactor*getHighestReward(newState))-lr);
	
	lastModel.addToHistory(std::pair<State, double>(lastState, targetValueForLastState));

	std::vector< std::vector<double> > input;
	std::vector< std::vector<double> > correctOutput;

	std::transform(lastModel.history.begin(), lastModel.history.end(), std::back_inserter(input), [](const std::pair<State, double> &entry) {
		return entry.first;
	});
	std::transform(lastModel.history.begin(), lastModel.history.end(), std::back_inserter(correctOutput), [](const std::pair<State, double> &entry) {
		return entry.second;
	});

	backprop.trainOnData(networks[lastAction], input, correctOutput);
}

std::vector<double> getModelRewards(State state); {
	std::vector<double> rewards;
	std::for_each(models.begin(), models.end(), [](Model model){
		rewards.push_back(models[a].network->getOutput(state)[0]);
	});

	return rewards;
}

double QLearn::getHighestReward(State state) {
	std::std::vector<double> rewards = getModelRewards(state);
	return *std::max_element(rewards.begin(), rewards.end());
}

void QLearn::storeQLearn(std::string filename) {
	std::ofstream output;
	output.open(filename.c_str(), std::ios::app);
	if(output.is_open()) {
		output << learningRate << " " << devaluationFactor << " " << numberOfActions << "\n";
		backprop.storeBackpropagationWithStream(&output);
		for(std::vector<net::NeuralNet *>::iterator a = networks.begin(); a != networks.end(); ++a) (*a)->storeNetWithStream(&output);

		output.close();
	} else {
		std::cout << "Could not store qlearn in file\n";
		throw 1;
	}
}
