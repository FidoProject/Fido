#include "WireFitRobot.h"

WireFitRobot::WireFitRobot() {
	int stateSize = 1;
	int numberOfHiddenLayers = 3;
	int numberOfNeuronsPerHiddenLayer = 12;
	int numberOfActions = 3, actionDimensions = 3;
	net::NeuralNet * network = new net::NeuralNet(stateSize, numberOfActions * (actionDimensions + 1), numberOfHiddenLayers, numberOfNeuronsPerHiddenLayer, "sigmoid");
	
	double backpropLearningRate = 0.4;
	double backpropMomentumTerm = 0.05;
	double backpropTargetError = 0.0005;
	int backpropMaximumIterations = 10000;
	net::Backpropagation backprop = net::Backpropagation(backpropLearningRate, backpropMomentumTerm, backpropTargetError, backpropMaximumIterations);

	double learningRate = 0.95;
	double devaluationFactor = 0.4;
	learner = net::WireFitQLearn(network, backprop, learningRate, devaluationFactor, actionDimensions, numberOfActions);

	boltzmanExplorationLevel = 8;
	explorationDevaluationPerTimestep = 0.6;
}

void WireFitRobot::run(int numberOfTimeSteps) {
	for(int a = 0; a < numberOfTimeSteps; a++) {
		waitForStateInput();
		std::vector<double> state = getState();
		std::vector<double> action = learner.chooseBoltzmanAction(state, boltzmanExplorationLevel);
		performAction(action);
		learner.applyReinforcementToLastAction(getReward(), state, 1);
		if(boltzmanExplorationLevel > 0.01) boltzmanExplorationLevel *= explorationDevaluationPerTimestep;
	}
}

void WireFitRobot::waitForStateInput() {
	std::string response;
	std::cout << "Respond when you finish inputing the state\n";
	std::cin >> response;
}

std::vector<double> WireFitRobot::getState() {
	std::vector<double> state;
	state.push_back(simulator.getVis());

	return state;
}

double WireFitRobot::getReward() {
	double reward = 0;

	std::cout << "Enter a reward.\n";
	std::cin >> reward;

	return reward;
}

void WireFitRobot::performAction(const std::vector<double> &action) {
	simulator.setLED(action[0] * 255, action[1] * 255, action[2] * 255, 255);
}