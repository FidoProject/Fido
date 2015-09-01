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
    
    scalingFactorToMillis = 0;
    smoothingFactor = 0.2;
    e = 0.01;
    gradientDescentErrorTarget = 0.0001;
    gradientDescentLearningRate = 0.5;
    gradientDescentMaxIterations = 10000;

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
        input >> scalingFactorToMillis >> smoothingFactor >> e >> gradientDescentErrorTarget >> gradientDescentLearningRate >> gradientDescentMaxIterations;
        
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
        std::cout << "Could not retrieve neural network from file\n";
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
        sumOfProbabilities += (exponentTerms[a] / sumOfExponentTerms);
        if(sumOfProbabilities >= determiner) {
            lastAction = wires[a].action;
            lastState = currentState;
            return wires[a].action;
        }
    }
    
    throw 1;
    
}

void WireFitQLearn::applyReinforcementToLastAction(double reward, std::vector<double> newState, double elapsedTimeMillis) {
    std::vector<Wire> controlWires = getWires(lastState);
    double scalingFactor = scalingFactorToMillis * elapsedTimeMillis;
    
    /// Update Q value according to adaptive learning
    double oldRewardForLastAction = getRewardUsingInterpolator(controlWires, lastAction);
    double feedback = ((1/scalingFactor)*(reward + (pow(devaluationFactor, scalingFactor)*highestReward(newState)))) + (1 - 1/scalingFactor) * highestReward(lastState);
    double newRewardForLastAction = ((1 - learningRate) * oldRewardForLastAction) + (learningRate*feedback);
   
    std::vector<Wire> newContolWires = newControlWires({lastAction, newRewardForLastAction}, controlWires);

	std::cout << "Reward received: " << reward << "\n";
	std::cout << oldRewardForLastAction << " " << newRewardForLastAction << "\n";

	std::cout << "old wires\n";
	for (int a = 0; a < controlWires.size(); a++) {
		std::cout << "	Wire: r: " << controlWires[a].reward << "; a: ";
		for (int b = 0; b < controlWires[a].action.size(); b++) {
			std::cout << controlWires[a].action[b] << " ";
		}
		std::cout << "\n";
	}

	std::cout << "new wires\n";
	for (int a = 0; a < newContolWires.size(); a++) {
		std::cout << "	Wire: r: " << newContolWires[a].reward << "; a: ";
		for (int b = 0; b < newContolWires[a].action.size(); b++) {
			std::cout << newContolWires[a].action[b] << " ";
		}
		std::cout << "\n";
	}
	
    backprop.trainOnData(network, {lastState}, {getRawOutput(newContolWires)});
}

void WireFitQLearn::storeWireFitQLearn(std::string filename) {
    std::ofstream output(filename);
    if(output.is_open()) {
        output << learningRate << " " << devaluationFactor << "\n";
        output << actionDimensions << " " << numberOfWires << "\n";
        output << scalingFactorToMillis << " " << smoothingFactor << " " << e << " " << gradientDescentErrorTarget << " " << gradientDescentLearningRate << " " << gradientDescentMaxIterations << "\n";
        
        for(int a = 0; a < lastAction.size(); a++) output << lastAction[a] << " ";
        output << "\n";
        
        backprop.storeBackpropagationWithStream(output);
        network->storeNetWithStream(output);
        
        output.close();
    } else {
        std::cout << "Could not retrieve neural network from file\n";
        throw 1;
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
    std::vector<double> bestAction;
    
    for(int a = 0; a < wires.size(); a++) {
        if(wires[a].reward > bestReward) {
            bestAction = wires[a].action;
            bestReward = wires[a].reward;
        }
    }
    
    return bestAction;
}

std::vector<Wire> WireFitQLearn::newControlWires(const Wire &correctWire, std::vector<Wire> controlWires) {
    double error = 0;
    int iterations = 0;
    
    do {
        for(int a = 0; a < controlWires.size(); a++) {
            std::cout << "change: " << gradientDescentLearningRate*rewardDerivative(correctWire.action, controlWires[a], controlWires) << "\n";
            controlWires[a].reward = controlWires[a].reward - gradientDescentLearningRate*rewardDerivative(correctWire.action, controlWires[a], controlWires);
            std::cout << "control: " << controlWires[a].reward << "\n";
            for(int b = 0; b < controlWires[a].action.size(); b++) {
                controlWires[a].action[b] = controlWires[a].action[b] - gradientDescentLearningRate*actionTermDerivative(correctWire.action[b], controlWires[a].action[b], correctWire.action, controlWires[a], controlWires);
            }
        }
        
        error = pow(correctWire.reward - getRewardUsingInterpolator(controlWires, correctWire.action), 2);
        iterations++;
    } while(error > gradientDescentErrorTarget && iterations < gradientDescentMaxIterations);

	std::cout << "Iterations: " << iterations << "\n";
    
    return controlWires;
}

double WireFitQLearn::rewardDerivative(const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires) {
    double maxRewardFromWires = -9999999;
    for(auto a = controlWires.begin(); a != controlWires.end(); ++a) if(a->reward > maxRewardFromWires) maxRewardFromWires = a->reward;
    
    std::cout << "new wires\n";
    for (int a = 0; a < controlWires.size(); a++) {
        std::cout << "	Wire: r: " << controlWires[a].reward << "; a: ";
        for (int b = 0; b < controlWires[a].action.size(); b++) {
            std::cout << controlWires[a].action[b] << " ";
        }
        std::cout << "\n";
    }
    
    double norm = normalize(controlWires, action), wsum = weightedSum(controlWires, action), distance = distanceBetweenWireAndAction(wire, action, maxRewardFromWires);
    
    return (norm * (distance + wire.reward*smoothingFactor) - wsum*smoothingFactor) / pow(norm * distance, 2);
}

double WireFitQLearn::actionTermDerivative(double actionTerm, double wireActionTerm, const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires) {
    double maxRewardFromWires = -9999999;
    for(auto a = controlWires.begin(); a != controlWires.end(); ++a) if(a->reward > maxRewardFromWires) maxRewardFromWires = a->reward;
    
    double norm = normalize(controlWires, action), wsum = weightedSum(controlWires, action), distance = distanceBetweenWireAndAction(wire, action, maxRewardFromWires);
    
    return ((wsum - norm*wire.reward) * 2 * (wireActionTerm - actionTerm)) / pow(norm*distance, 2);
}

double WireFitQLearn::getRewardUsingInterpolator(const std::vector<Wire> &controlWires, const std::vector<double> &action) {
    return weightedSum(controlWires, action) / normalize(controlWires, action);
}

double WireFitQLearn::distanceBetweenWireAndAction(const Wire &wire, const std::vector<double> &action, double maxReward) {
    double euclideanNorm = 0;
    for(int a = 0; a < action.size(); a++) euclideanNorm += pow(action[a] - wire.action[a], 2);
    euclideanNorm = sqrt(euclideanNorm);

    return pow(euclideanNorm, 2) + smoothingFactor*(maxReward - wire.reward) + e;
}

double WireFitQLearn::weightedSum(const std::vector<Wire> &wires, const std::vector<double> &action) {
    double maxRewardFromWires = -9999999;
    for(auto a = wires.begin(); a != wires.end(); ++a) if(a->reward > maxRewardFromWires) maxRewardFromWires = a->reward;
    
    double answer = 0;
    for(auto a = wires.begin(); a != wires.end(); ++a) {
        answer += a->reward / distanceBetweenWireAndAction(*a, action, maxRewardFromWires);
    }
    
    return answer;
}

double WireFitQLearn::normalize(const std::vector<Wire> &wires, const std::vector<double> &action) {
    double maxRewardFromWires = -9999999;
    for(auto a = wires.begin(); a != wires.end(); ++a) if(a->reward > maxRewardFromWires) maxRewardFromWires = a->reward;
    
    double answer = 0;
    for(auto a = wires.begin(); a != wires.end(); ++a) {
        answer += 1.0 / distanceBetweenWireAndAction(*a, action, maxRewardFromWires);
    }
    
    return answer;
}
