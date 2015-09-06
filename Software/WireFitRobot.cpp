#include "WireFitRobot.h"

WireFitRobot::WireFitRobot() {
	int stateSize = 1;
	int numberOfHiddenLayers = 3;
	int numberOfNeuronsPerHiddenLayer = 4;
	int numberOfActions = 3, actionDimensions = 1;
	net::NeuralNet * network = new net::NeuralNet(stateSize, numberOfActions * (actionDimensions + 1), numberOfHiddenLayers, numberOfNeuronsPerHiddenLayer, "sigmoid");
	network->setOutputActivationFunction("simpleLinear");
	network->setHiddenActivationFunction("simpleLinear");

	double backpropLearningRate = 0.9;
	double backpropMomentumTerm = 0;
	double backpropTargetError = 0.001;
	int backpropMaximumIterations = 10000;
	net::Backpropagation backprop = net::Backpropagation(backpropLearningRate, backpropMomentumTerm, backpropTargetError, backpropMaximumIterations);
	backprop.setDerivedOutputActivationFunction("simpleLinear");
	backprop.setDerivedHiddenActivationFunction("simpleLinear");

	double learningRate = 0.95;
	double devaluationFactor = 0.4;
	learner = net::WireFitQLearn(network, backprop, learningRate, devaluationFactor, actionDimensions, numberOfActions);

	boltzmanExplorationLevel = 8;
	explorationDevaluationPerTimestep = 0.6;
}

void WireFitRobot::run(int numberOfTimeSteps) {
	for(int a = 0; a < numberOfTimeSteps; a++) {
		//waitForStateInput();
		std::cout << "a: " << a << "\n";
		std::vector<double> state = getState();
		std::vector<double> action = learner.chooseRandomAction(state, { 0 }, { 1 });
		performAction(action);
		learner.applyReinforcementToLastAction(std::max(std::min(action[0], 1.0), 0.0), state, 1);
		if(boltzmanExplorationLevel > 0.01) boltzmanExplorationLevel *= explorationDevaluationPerTimestep;
	}

	learner.graphInterpolatorFunction(learner.getWires({ 0 }), 0, 1, 0.5);
	
	while (true) {
		waitForStateInput();
		std::vector<double> state = getState();
		std::vector<double> action = learner.chooseBestAction(state);
		performAction(action);
		learner.applyReinforcementToLastAction(getReward(), state, 1);
		if (boltzmanExplorationLevel > 0.01) boltzmanExplorationLevel *= explorationDevaluationPerTimestep;
	}
}

void WireFitRobot::waitForStateInput() {
	std::string response;
	std::cout << "Respond when you finish inputing the state\n";
	std::cin >> response;
}

std::vector<double> WireFitRobot::getState() {
	std::vector<double> state;
	state.push_back(0);

	return state;
}

double WireFitRobot::getReward() {
	double reward = 0;

	std::cout << "Enter a reward.\n";
	std::cin >> reward;

	return reward;
}

void WireFitRobot::performAction(const std::vector<double> &action) {
	simulator.setMotors(action[0] * 100, 0);
}