#include "Robot.h"

Robot::Robot(Task *task_) {
	task = task_;

	int dummy;

	task->init(&simulator);
	task->getRobotParameters(&stateSize, &actionDimensions, &dummy, &numberOfNeuronsPerHiddenLayer, &numberOfHiddenLayers, &boltzmanExplorationLevel, &explorationDevaluationPerTimestep, &minAction, &maxAction, &baseOfDimensions);

	net::NeuralNet * network = new net::NeuralNet(stateSize, 1, numberOfHiddenLayers, numberOfNeuronsPerHiddenLayer, "sigmoid");
	network->setOutputActivationFunction("simpleLinear");

	double backpropLearningRate = 0.1;
	double backpropMomentumTerm = 0;
	double backpropTargetError = 0.001;
	int backpropMaximumIterations = 10000;
	net::Backpropagation backprop = net::Backpropagation(backpropLearningRate, backpropMomentumTerm, backpropTargetError, backpropMaximumIterations);
	backprop.setDerivedOutputActivationFunction("simpleLinear");

	double learningRate = 0.95;
	double devaluationFactor = 0.4;
	learner = net::QLearn(network, backprop, learningRate, devaluationFactor, pow(baseOfDimensions, actionDimensions));

}

void Robot::run(int numberOfTimeSteps) {
	task->reset();
	for(int a = 0; a < numberOfTimeSteps; a++) {
		boltzmanExplorationLevel *= explorationDevaluationPerTimestep;

		std::vector<double> state = task->getState();

		int action = learner.chooseBoltzmanAction(state, boltzmanExplorationLevel);
		double reward = task->performAction(getAction(action));

		learner.applyReinforcementToLastAction(reward, state);

		if (task->isTaskDone() == true) break;
	}
}

std::vector<double> Robot::getAction(int action) {
	std::vector<int> iteratorVector(actionDimensions);
	for (int a = 0; a < action; a++) {
		iteratorVector[iteratorVector.size() - 1]++;
		for (int iteratorVectorIndex = iteratorVector.size() - 1; iteratorVectorIndex >= 0; iteratorVectorIndex--) {
			if (iteratorVector[iteratorVectorIndex] >= baseOfDimensions) {
				iteratorVector[iteratorVectorIndex] = 0;
				if (iteratorVectorIndex > 0)iteratorVector[iteratorVectorIndex - 1]++;
			}
		}
	}
	
	std::vector<double> scaleVector(minAction.size());
	for (int a = 0; a < minAction.size(); a++) {
		scaleVector[a] = (maxAction[a] - minAction[a]) / ((double)baseOfDimensions - 1);
	}

	std::vector<double> actionVector(actionDimensions);
	for (int actionDimension = 0; actionDimension < minAction.size(); actionDimension++) {
		actionVector.push_back(iteratorVector[actionDimension] * scaleVector[actionDimension] + minAction[actionDimension]);
	}

	return actionVector;
}