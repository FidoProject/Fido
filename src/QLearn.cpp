#include "../include/QLearn.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <algorithm>

#include "../include/NeuralNet.h"

using namespace rl;

QLearn::QLearn(net::NeuralNet *modelNetwork, net::Backpropagation backprop_, double learningRate_, double devaluationFactor_, std::vector<Action> possibleActions_) {
	backprop = backprop_;
	learningRate = learningRate_;
	devaluationFactor = devaluationFactor_;

	models = std::vector<Model>(possibleActions_.size());
	for(unsigned int a = 0; a < possibleActions_.size(); a++) models[a] = (Model(new net::NeuralNet(modelNetwork), possibleActions_[a]));
}

QLearn::QLearn(std::vector<Model> models_, net::Backpropagation backprop_, double learningRate_, double devaluationFactor_) {
	models = models_;
	backprop = backprop_;
	learningRate = learningRate_;
	devaluationFactor = devaluationFactor_;
}

QLearn::QLearn() {

}

Action QLearn::chooseBestAction(State currentState) {
	lastState = currentState;

	std::vector<double> rewards = getModelRewards(currentState);
	lastModel = models[std::max_element(rewards.begin(), rewards.end()) - rewards.begin()];
	return lastModel.action;
}

Action QLearn::chooseBoltzmanAction(State currentState, double explorationConstant) {
	if(explorationConstant < 0.01) {
		std::cout << "Exploration constant (" << explorationConstant << ") is below 0.01. This is too low!. Will cause integer over flow. Assuming an explorationConstant of 0.01 instead.";
		explorationConstant = 0.01;
	}

	double determiner = (double)rand() / (double)RAND_MAX;

	std::vector<double> rewards = getModelRewards(currentState);

	std::vector<double> exponentTerms(0);
	double sumOfExponentTerms = 0;
	std::for_each(rewards.begin(), rewards.end(), [&](double reward){
		double exponentTerm = exp(reward / explorationConstant);
		exponentTerms.push_back(exponentTerm);
		sumOfExponentTerms += exponentTerm;
	});

	double sumOfProbabilities = 0;
	for(unsigned int a = 0; a < exponentTerms.size(); a++) {
		sumOfProbabilities += (exponentTerms[a] / sumOfExponentTerms);
		if(sumOfProbabilities >= determiner) {
			lastModel = models[a];
			lastState = currentState;
			return lastModel.action;
		}
	}

	/// Incase a floating point error resulted in no action
	std::cout << "Floating point error when choosing an action using a Boltzmann selection policy! Choosing last action.";

	lastModel = models[models.size() - 1];
	lastState = currentState;
	return lastModel.action;
}

void QLearn::applyReinforcementToLastAction(double reward, State newState) {
	if(lastState.size() == 0) {
		std::cout << "Called applyReinforcementToLastAction before an action had been selected! Because of this, this function call will be ignored.";
	}

	double lr = lastModel.network->getOutput(lastState)[0];
	double targetValueForLastState = lr + learningRate*(reward+(devaluationFactor*getHighestReward(newState))-lr);

	lastModel.addToHistory(std::pair<State, double>(lastState, targetValueForLastState));

	std::vector< std::vector<double> > input;
	std::vector< std::vector<double> > correctOutput;

	std::transform(lastModel.history.begin(), lastModel.history.end(), std::back_inserter(input), [](std::pair<State, double> entry) {
		return entry.first;
	});
	std::transform(lastModel.history.begin(), lastModel.history.end(), std::back_inserter(correctOutput), [](std::pair<State, double> entry) {
		std::vector<double> returnVal = {entry.second};
		return returnVal;
	});

	backprop.train(lastModel.network, input, correctOutput);
}

void QLearn::reset() {
	std::for_each(models.begin(), models.end(), [&](Model model) {
		model.network->randomizeWeights();
		model.history.clear();
	});
}

std::vector<double> QLearn::getModelRewards(State state) {
	std::vector<double> rewards;
	std::for_each(models.begin(), models.end(), [&](Model model){
		rewards.push_back(model.network->getOutput(state)[0]);
	});

	return rewards;
}

double QLearn::getHighestReward(State state) {
	std::vector<double> rewards = getModelRewards(state);
	return *std::max_element(rewards.begin(), rewards.end());
}

void QLearn::store(std::ofstream *output) {
	*output << "QLearn ";
}
