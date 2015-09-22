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

void Robot::run(int trials, int numberOfTimeSteps) {
	std::vector<int> results(trials);
	for (int currentTrial = 0; currentTrial < trials; currentTrial++) {
		task->reset();

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

		int a;
		for (a = 0; a < numberOfTimeSteps; a++) {
			boltzmanExplorationLevel *= explorationDevaluationPerTimestep;

			std::vector<double> state = task->getState();

			std::vector<double> action = getAction(learner.chooseBoltzmanAction(state, boltzmanExplorationLevel));
			double reward = task->performAction(action);

			learner.applyReinforcementToLastAction(reward, state);

			if (task->isTaskDone() == true) break;

			for (int a = 0; a < 6; a++) {
				action = getAction(learner.chooseBoltzmanAction(task->getState(), boltzmanExplorationLevel));
				task->performAction(action);
			}
		}
		std::cout << "iter: " << a << "\n";
		results[currentTrial] = a;
	}

	WireFitRobot::printStats(std::vector<double>(results.begin(), results.end()));
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
		actionVector[actionDimension] = iteratorVector[actionDimension] * scaleVector[actionDimension] + minAction[actionDimension];
	}

	return actionVector;
}