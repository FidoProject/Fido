#include "WireFitQLearn.h"

using namespace net;

WireFitQLearn::WireFitQLearn(NeuralNet *modelNetwork, Backpropagation backprop_, double learningRate_, double devaluationFactor_, int actionDimensions_, int numberOfWires_) {
    backprop = backprop_;
    learningRate = learningRate_;
    devaluationFactor = devaluationFactor_;
    actionDimensions = actionDimensions_;
    numberOfWires = numberOfWires_;
    lastAction = std::vector<double>(actionDimensions);
    network = modelNetwork;
    
    scalingFactorToMillis = 1;
    smoothingFactor = 0;
    e = 0.01;
    controlPointsGDErrorTarget = 0.001;
    controlPointsGDLearningRate = 0.1;
    controlPointsGDMaxIterations = 10000;

    if(network->numOutputs != numberOfWires * (actionDimensions+1)) {
        std::cout << "Neural network incorrectly formatted!\n";
        throw 1;
    }
}

WireFitQLearn::WireFitQLearn(std::string filename) {
    std::ifstream input(filename);
    if(input.is_open()) {
        input >> learningRate >> devaluationFactor;
        input >> actionDimensions >> numberOfWires;
        input >> scalingFactorToMillis >> smoothingFactor >> e >> controlPointsGDErrorTarget >> controlPointsGDLearningRate >> controlPointsGDMaxIterations;
        
        lastAction = std::vector<double>(actionDimensions);
        for(int a = 0; a < actionDimensions; a++) {
            double temp;
            input >> temp;
            lastAction[a] = temp;
            
        }
        
        backprop = Backpropagation(input);
        network = new NeuralNet(input);
        
        input.close();

        if(network->numOutputs != numberOfWires * (actionDimensions+1)) {
            std::cout << "Neural network incorrectly formatted!\n";
            throw 1;
        }
    } else {
        std::cout << "Could not retrieve wirefitqlearn from file\n";
        throw 1;
    }
}

WireFitQLearn::WireFitQLearn() { }

std::vector<double> WireFitQLearn::chooseBestAction(std::vector<double> currentState) {
    std::vector<double> action = bestAction(currentState);
    lastAction = action;
    lastState = currentState;
    return action;
}

std::vector<double> WireFitQLearn::chooseBoltzmanAction(std::vector<double> currentState, double explorationConstant) {
    double determiner = (double)rand() / (double)RAND_MAX;
    std::vector<double> exponentTerms;
    double sumOfExponentTerms = 0;
    std::vector<Wire> wires = getWires(currentState);
    
    for(int a = 0; a < wires.size(); a++) {
        double exponentTerm = exp(wires[a].reward / explorationConstant);
        exponentTerms.push_back(exponentTerm);
        sumOfExponentTerms += exponentTerm;
    }
    
    double sumOfProbabilities = 0;
    for(int a = 0; a < wires.size(); a++) {
		std::cout << "ex: " << exponentTerms[a] << "; sum: " << sumOfExponentTerms << "\n";
        sumOfProbabilities += (exponentTerms[a] / sumOfExponentTerms);
		std::cout << "prop: " << sumOfProbabilities << "\n";
        if(sumOfProbabilities >= determiner) {
            lastAction = wires[a].action;
            lastState = currentState;
            return wires[a].action;
        }
    }
    
    throw 1;
    
}

std::vector<double> WireFitQLearn::chooseRandomAction(const std::vector<double> &state, const std::vector<double> &minAction, const std::vector<double> &maxAction) {
	if (minAction.size() != maxAction.size()) {
		std::cout << "Min action and max action sizes don't match\n";
		throw 1;
	}
	
	std::vector<double> action(minAction.size());
	for (int a = 0; a < minAction.size(); a++) action[a] = minAction[a] + ((double)rand() / RAND_MAX) * (maxAction[a] - minAction[a]);
	
	lastAction = action;
	lastState = state;

	return action;
}

void WireFitQLearn::applyReinforcementToLastAction(double reward, std::vector<double> newState, double elapsedTimeMillis) {
    std::vector<Wire> controlWires = getWires(lastState);
    double scalingFactor = scalingFactorToMillis * elapsedTimeMillis;
    
    /// Update Q value according to adaptive learning
    double oldRewardForLastAction = getRewardUsingInterpolator(controlWires, lastAction);
    //double feedback = ((1/scalingFactor)*( reward + (pow(devaluationFactor, scalingFactor)*highestReward(newState)) )) + (1 - 1/scalingFactor) * highestReward(lastState);
	double feedback = reward + devaluationFactor * highestReward(newState);
    double newRewardForLastAction = ((1 - learningRate) * oldRewardForLastAction) + (learningRate*feedback);

	Wire correctWire = {lastAction, newRewardForLastAction};
	std::vector<Wire> newContolWires = newControlWires(correctWire, controlWires);

    backprop.trainOnData(network, {lastState}, {getRawOutput(newContolWires)});
	controlWires = getWires(lastState);
}

void WireFitQLearn::trainOnHistoricalData(const std::vector< std::vector<double> > &actions,
	const std::vector< std::vector<double> > &oldStates,
	const std::vector<double> &immediateRewards,
	const std::vector< std::vector<double> > &newStates,
	const std::vector<double> &elapsedTimes,
	int numberOfIterations) {

	if(actions.size() != oldStates.size() || oldStates.size() != immediateRewards.size() || immediateRewards.size() != newStates.size() || newStates.size() != elapsedTimes.size()) {
		std::cout << "Varying lengths of historical data vectors\n";
		throw 1;
	}
	
	for(int a = 0; a < numberOfIterations; a++) {
		for(int b = 0; b < actions.size(); b++) {
			lastState = oldStates[b];
			lastAction = actions[b];
			applyReinforcementToLastAction(immediateRewards[b], newStates[b], elapsedTimes[b]);
		}

		std::vector<double> bestAction = chooseBestAction(oldStates[oldStates.size() - 1]);
		std::cout << "a: " << a << "; top: " << std::max(1 - abs(bestAction[0] - bestAction[1]), 0.0) << "\n";
	}
}

void WireFitQLearn::storeWireFitQLearn(std::string filename) {
    std::ofstream output(filename);
    if(output.is_open()) {
        output << learningRate << " " << devaluationFactor << "\n";
        output << actionDimensions << " " << numberOfWires << "\n";
        output << scalingFactorToMillis << " " << smoothingFactor << " " << e << " " << controlPointsGDErrorTarget << " " << controlPointsGDLearningRate << " " << controlPointsGDMaxIterations << "\n";
        
        for(int a = 0; a < lastAction.size(); a++) output << lastAction[a] << " ";
        output << "\n";
        
        backprop.storeBackpropagationWithStream(output);
        network->storeNetWithStream(output);
        
        output.close();
    } else {
        std::cout << "Could not store wirefitqlearn in file\n";
        throw 1;
    }
}

void WireFitQLearn::graphInterpolatorFunction(const std::vector<Wire> &controlWires, double minAction, double maxAction, double targetAction) {
	double xSize = 1000, ySize = 800;
	sf::RenderWindow window(sf::VideoMode(xSize, ySize), "Interpolator Graph");
	double numberOfDots = 100;

	for(int a = 0; a < controlWires.size(); a++) {
		if(controlWires[a].action[0] > maxAction) maxAction = controlWires[a].action[0];
		if(controlWires[a].action[0] < minAction) minAction = controlWires[a].action[0];
	}
	
	double increment = (maxAction - minAction) / numberOfDots;
	double xScale = (xSize - 60) / (maxAction - minAction);

	double maxReward = -99999999, minReward = 99999999;
	std::vector<double> rewards(numberOfDots);
	for(int a = 0; a < numberOfDots; a++) {
		rewards[a] = getRewardUsingInterpolator(controlWires, {a*increment});
		if(rewards[a] > maxReward) maxReward = rewards[a];
		if(rewards[a] < minReward) minReward = rewards[a];
	}
	for(int a = 0; a < controlWires.size(); a++) {
		if(controlWires[a].reward > maxReward) maxReward = controlWires[a].reward;
		if(controlWires[a].reward < minReward) minReward = controlWires[a].reward;
	}

	double yScale = (ySize - 60) / (maxReward - minReward);

	window.clear();
	sf::CircleShape shape(5.f);
	shape.setFillColor(sf::Color::Green);
	for(int a = 0; a < numberOfDots; a++) {
		shape.setPosition((a*increment - minAction) * xScale + 30, (maxReward - rewards[a])*yScale + 30);
		window.draw(shape);
	}

	shape.setRadius(10.f);
	shape.setFillColor(sf::Color::Red);
	for(int a = 0; a < controlWires.size(); a++) {
		shape.setPosition((controlWires[a].action[0] - minAction) * xScale + 30, (maxReward - controlWires[a].reward)*yScale + 30);
		window.draw(shape);
	}

	shape.setFillColor(sf::Color::Magenta);
	shape.setPosition((targetAction - minAction) * xScale + 30, (maxReward - getRewardUsingInterpolator(controlWires, {targetAction}))*yScale + 30);
	window.draw(shape);

	window.display();
	

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
		}
	}
}

std::vector<Wire> WireFitQLearn::getWires(std::vector<double> state) {
    std::vector<Wire> wires(numberOfWires);
    std::vector<double> rawOutput = network->getOutput(state);
    
    for(int a = 0; a < numberOfWires; a++) {
        int currentIndex = a * (actionDimensions + 1);
        Wire wire;
        wire.action = std::vector<double>(actionDimensions);
        for(int b = 0; b < actionDimensions; b++) wire.action[b] = rawOutput[currentIndex + b];
        
        wire.reward = rawOutput[currentIndex + actionDimensions];
        
        wires[a] = wire;
    }
    
    return wires;
}

std::vector<double> WireFitQLearn::getRawOutput(std::vector<Wire> wires) {
    std::vector<double> rawOutput(wires.size() * (actionDimensions + 1));
    
    for(int a = 0; a < wires.size(); a++) {
        int currentIndex = a * (actionDimensions + 1);
        Wire wire = wires[a];
        for(int b = 0; b < wire.action.size(); b++) rawOutput[currentIndex + b] = wire.action[b];
        
        rawOutput[currentIndex + wire.action.size()] = wire.reward;
    }
    
    return rawOutput;
}


double WireFitQLearn::highestReward(std::vector<double> state) {
    std::vector<Wire> wires = getWires(state);
    double bestReward = -99999;
    
    for(int a = 0; a < wires.size(); a++) if(wires[a].reward > bestReward) bestReward = wires[a].reward;
    
    return bestReward;
}

std::vector<double> WireFitQLearn::bestAction(std::vector<double> state) {
    std::vector<Wire> wires = getWires(state);
    double bestReward = -99999;
    std::vector<double> *bestAction = 0;
    
    for(int a = 0; a < wires.size(); a++) {
        if(wires[a].reward > bestReward) {
            bestAction = &wires[a].action;
            bestReward = wires[a].reward;
        }
    }
    
    return *bestAction;
}

std::vector<Wire> WireFitQLearn::newControlWires(const Wire &correctWire, std::vector<Wire> controlWires) {
    double error = 0;
    int iterations = 0;
    
    do {
        for(int a = 0; a < controlWires.size(); a++) {
            double deltaReward = -2 * (-getRewardUsingInterpolator(controlWires, correctWire.action) + correctWire.reward)*rewardDerivative(correctWire.action, controlWires[a], controlWires);
            controlWires[a].reward = controlWires[a].reward - controlPointsGDLearningRate*deltaReward;
            for(int b = 0; b < controlWires[a].action.size(); b++) {
				double deltaActionTerm = -2 * (-getRewardUsingInterpolator(controlWires, correctWire.action) + correctWire.reward)*actionTermDerivative(correctWire.action[b], controlWires[a].action[b], correctWire.action, controlWires[a], controlWires);
                controlWires[a].action[b] = controlWires[a].action[b] - controlPointsGDLearningRate*deltaActionTerm;
            }
        }
        
        error = pow(correctWire.reward - getRewardUsingInterpolator(controlWires, correctWire.action), 2);
        iterations++;
    } while(error > controlPointsGDErrorTarget && iterations < controlPointsGDMaxIterations);

    return controlWires;

}

double WireFitQLearn::rewardDerivative(const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires) {
    double maxReward = -9999999;
    for(auto a = controlWires.begin(); a != controlWires.end(); ++a) if(a->reward > maxReward) maxReward = a->reward;
    
    double norm = normalize(controlWires, action, maxReward), wsum = weightedSum(controlWires, action, maxReward), distance = distanceBetweenWireAndAction(wire, action, maxReward);
    
    return (norm * (distance + wire.reward*smoothingFactor) - wsum*smoothingFactor) / pow(norm * distance, 2);
}

double WireFitQLearn::actionTermDerivative(double actionTerm, double wireActionTerm, const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires) {
    double maxReward = -9999999;
    for(auto a = controlWires.begin(); a != controlWires.end(); ++a) if(a->reward > maxReward) maxReward = a->reward;
    
    double norm = normalize(controlWires, action, maxReward), wsum = weightedSum(controlWires, action, maxReward), distance = distanceBetweenWireAndAction(wire, action, maxReward);
    
    return ((wsum - norm*wire.reward) * 2 * (wireActionTerm - actionTerm)) / pow(norm*distance, 2);
}

double WireFitQLearn::getRewardUsingInterpolator(const std::vector<Wire> &controlWires, const std::vector<double> &action) {
	double maxReward = -9999999;
	for (auto a = controlWires.begin(); a != controlWires.end(); ++a) if (a->reward > maxReward) maxReward = a->reward;

    return weightedSum(controlWires, action, maxReward) / normalize(controlWires, action, maxReward);
}

double WireFitQLearn::distanceBetweenWireAndAction(const Wire &wire, const std::vector<double> &action, double maxReward) {
    double euclideanNorm = 0;
    for(int a = 0; a < action.size(); a++) euclideanNorm += pow(action[a] - wire.action[a], 2);
    euclideanNorm = sqrt(euclideanNorm);

    return pow(euclideanNorm, 2) + smoothingFactor*(maxReward - wire.reward) + e;
}

double WireFitQLearn::weightedSum(const std::vector<Wire> &wires, const std::vector<double> &action, double maxReward) {
    double answer = 0;
    for(auto a = wires.begin(); a != wires.end(); ++a) {
        answer += a->reward / distanceBetweenWireAndAction(*a, action, maxReward);
    }
    
    return answer;
}

double WireFitQLearn::normalize(const std::vector<Wire> &wires, const std::vector<double> &action, double maxReward) {
    double answer = 0;
    for(auto a = wires.begin(); a != wires.end(); ++a) {
        answer += 1.0 / distanceBetweenWireAndAction(*a, action, maxReward);
    }
    
    return answer;
}
