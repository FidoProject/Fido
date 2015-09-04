#include "Robot.h"

Robot::Robot() {
	int stateSize = 1;
	int numberOfHiddenLayers = 1;
	int numberOfNeuronsPerHiddenLayer = 2;
	net::NeuralNet * network = new net::NeuralNet(stateSize, 1, numberOfHiddenLayers, numberOfNeuronsPerHiddenLayer, "sigmoid");
	network->setOutputActivationFunction("simpleLinear");
	
	double backpropLearningRate = 0.4;
	double backpropMomentumTerm = 0.05;
	double backpropTargetError = 0.01;
	int backpropMaximumIterations = 10000;
	net::Backpropagation backprop = net::Backpropagation(backpropLearningRate, backpropMomentumTerm, backpropTargetError, backpropMaximumIterations);
	backprop.setDerivedOutputActivationFunction("simpleLinear");

	double learningRate = 0.95;
	double devaluationFactor = 0.4;
	double numberOfActions = 2;
	learner = net::QLearn(network, backprop, learningRate, devaluationFactor, numberOfActions);

	boltzmanExplorationLevel = 8;
	explorationDevaluationPerTimestep = 0.6;
}

void Robot::run(int numberOfTimeSteps) {
	for(int a = 0; a < numberOfTimeSteps; a++) {
		waitForStateInput();
		std::vector<double> state = getState();
		int action = learner.chooseBoltzmanAction(state, boltzmanExplorationLevel);
		performAction(action);
		learner.applyReinforcementToLastAction(getReward(), state);
		if(boltzmanExplorationLevel > 0.01) boltzmanExplorationLevel *= explorationDevaluationPerTimestep;
	}
}

void Robot::waitForStateInput() {
	std::string response;
	std::cout << "Respond when you finish inputing the state\n";
	std::cin >> response;
}

std::vector<double> Robot::getState() {
	std::vector<double> state;
	state.push_back(simulator.getVis());

	return state;
}

double Robot::getReward() {
	double reward = 0;

	std::cout << "Enter a reward.\n";
	std::cin >> reward;

	return reward;
}

void Robot::performAction(int action) {
	if(action == 0) {
		simulator.setLED(255, 0, 0, 255);
	} else if(action == 1) {
		simulator.setLED(0, 0, 0, 0);
	}
}