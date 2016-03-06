#include "QLearn.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <chrono>
#include <thread>

#include "NeuralNet.h"

using namespace net;

QLearn::QLearn(NeuralNet *modelNetwork, Backpropagation backprop_, double learningRate_, double devaluationFactor_, int numberOfActions_) {
	backprop = backprop_;
	learningRate = learningRate_;
	devaluationFactor = devaluationFactor_;
	numberOfActions = numberOfActions_;
	lastAction = -1;
    lastReward = -1;

	for(int a = 0; a < numberOfActions; a++) networks.push_back(new net::NeuralNet(modelNetwork));

	history = std::vector< std::vector< std::pair<std::vector<double>, double> > >(numberOfActions, std::vector< std::pair<std::vector<double>, double> >(0));
}

QLearn::QLearn(std::vector<NeuralNet *> networks_, Backpropagation backprop_, double learningRate_, double devaluationFactor_) {
	networks = networks_;
	backprop = backprop_;
	learningRate = learningRate_;
	devaluationFactor = devaluationFactor_;
	numberOfActions = (int)networks.size();
	lastAction = -1;
    lastReward = -1;

    history = std::vector< std::vector< std::pair<std::vector<double>, double> > >(numberOfActions, std::vector< std::pair<std::vector<double>, double> >(0));
}

QLearn::QLearn(std::string filename) {
	std::ifstream input;
	input.open(filename.c_str(), std::ifstream::in);
	if(input.is_open()) {
		input >> learningRate >> devaluationFactor >> numberOfActions >> lastAction >> lastReward;
		backprop = Backpropagation(&input);
		for(int a = 0; a < numberOfActions; a++) networks.push_back(new NeuralNet(&input));

		input.close();
	} else {
		std::cout << "Could not retrieve qlearn from file\n";
		throw 1;
	}
}

QLearn::QLearn() { }

unsigned int QLearn::chooseBestAction(std::vector<double> currentState) {
    getBestActionAndReward(currentState, &lastAction, &lastReward);
	lastState = currentState;
	return lastAction;
}

unsigned int QLearn::chooseBoltzmanAction(std::vector<double> currentState, double explorationConstant) {
	double determiner = (double)rand() / (double)RAND_MAX;
    std::vector<double> rewards(networks.size());
	std::vector<double> exponentTerms(networks.size());
	double sumOfExponentTerms = 0;

	if (explorationConstant < 0.01) explorationConstant = 0.01;

	for(int a = 0; a < networks.size(); a++) {
		double reward = networks[a]->getOutput(currentState)[0];
		double exponentTerm = exp(reward / explorationConstant);
        
        rewards[a] = reward;
		exponentTerms[a] = exponentTerm;
		sumOfExponentTerms += exponentTerm;
	}

	double sumOfProbabilities = 0;
	for(int a = 0; a < networks.size(); a++) {
		sumOfProbabilities += (exponentTerms[a] / sumOfExponentTerms);
		if(sumOfProbabilities >= determiner) {
			lastAction = a;
            lastReward = rewards[a];
			lastState = currentState;
			return a;
		}
	}

	/// Incase a floating point error resulted in no action
	lastAction = (unsigned int)networks.size() - 1;
	lastState = currentState;
	return lastAction;
}

void QLearn::applyReinforcementToLastAction(double reward, std::vector<double> newState) {
	if(lastAction == -1) return;
    
    double lr = networks[lastAction]->getOutput(lastState)[0];
	//double feedback = (reward + (devaluationFactor*highestReward(newState)));
	//double targetValueForLastState = ((1 - learningRate) * lastReward) + (learningRate*feedback);
	double targetValueForLastState = lr + learningRate*(reward+(devaluationFactor*highestReward(newState))-lr);
	
	if(history.size() < 10) history[lastAction].push_back(std::pair<std::vector<double>, double>(lastState, targetValueForLastState));	

	std::vector< std::vector<double> > input = std::vector< std::vector<double> >(history[lastAction].size());
	for(int a = 0; a < history[lastAction].size(); a++) input[a] = history[lastAction][a].first;
	std::vector< std::vector<double> > correctOutput = std::vector< std::vector<double> >(history[lastAction].size());
	for(int a = 0; a < history[lastAction].size(); a++) correctOutput[a] = { history[lastAction][a].second };

	std::cout << "; LastState: " << lastState[0] << "; targetValueForLastState: " << targetValueForLastState << "; lastReward: " << lr << "\n";

	backprop.trainOnData(networks[lastAction], input, correctOutput);
}

void QLearn::getBestActionAndReward(std::vector<double> state, unsigned int *bestAction, double *bestReward) {
	*bestAction = 0;
	*bestReward = -99999;

	for(int a = 0; a < networks.size(); a++) {
		double reward = networks[a]->getOutput(state)[0];
		if(reward > *bestReward) {
			*bestAction = a;
			*bestReward = reward;
		}
	}
}

double QLearn::highestReward(std::vector<double> state) {
	unsigned int bestAction;
	double bestReward;
	getBestActionAndReward(state, &bestAction, &bestReward);

	return bestReward;
}

int QLearn::bestAction(std::vector<double> state) {
	unsigned int bestAction;
	double bestReward;
	getBestActionAndReward(state, &bestAction, &bestReward);

	return bestAction;
}

void QLearn::storeQLearn(std::string filename) {
	std::ofstream output;
	output.open(filename.c_str(), std::ios::app);
	if(output.is_open()) {
		output << learningRate << " " << devaluationFactor << " " << numberOfActions << " " << lastAction << " " << lastReward << "\n";
		backprop.storeBackpropagationWithStream(&output);
		for(std::vector<net::NeuralNet *>::iterator a = networks.begin(); a != networks.end(); ++a) (*a)->storeNetWithStream(&output);

		output.close();
	} else {
		std::cout << "Could not store qlearn in file\n";
		throw 1;
	}
}
