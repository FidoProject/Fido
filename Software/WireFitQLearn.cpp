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
    scalingFactorToMillis = 0.5;
}

WireFitQLearn::WireFitQLearn(std::string filename) {
    std::ifstream input(filename);
    if(input.is_open()) {
        input >> learningRate >> devaluationFactor >> scalingFactorToMillis;
        input >> actionDimensions >> numberOfWires;
        
        lastAction = std::vector<double>(actionDimensions);
        for(int a = 0; a < actionDimensions; a++) {
            double temp;
            input >> temp;
            lastAction[a] = temp;
            
        }
        
        backprop = Backpropagation(input);
        network = new NeuralNet(input);
        
        input.close();
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
    std::vector<Wire> wires = getWires(lastState);
    double scalingFactor = scalingFactorToMillis * elapsedTimeMillis;
    
    /// Update Q value according to adaptive learning
    double oldValue = wireFitting(lastState, wires, lastAction);
    double feedback = ((1/scalingFactor)*(reward + (pow(devaluationFactor, scalingFactor)*highestReward(newState)))) + (1 - 1/scalingFactor) * highestReward(lastState);
    double newQ = ((1 - learningRate) * oldValue) + (learningRate*feedback);
    
    
}

void WireFitQLearn::storeWireFitQLearn(std::string filename) {
    std::ofstream output(filename);
    if(output.is_open()) {
        output << learningRate << " " << devaluationFactor << " " << scalingFactorToMillis << "\n";
        output << actionDimensions << " " << numberOfWires << "\n";
        
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
        for(int b = 0; b < actionDimensions; b++) wire.action[a] = rawOutput[currentIndex + b];
        
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

double WireFitQLearn::wireFitting(std::vector<double> state, std::vector<Wire> wires, std::vector<double> action) {
    return weightedsum(state, wires, action) / normalize(state, wires, action);
}

double WireFitQLearn::distance(std::vector<double> &state, Wire &wire, std::vector<double> &action, double maxRewardFromWires) {
    double c = 1;
    
    double euclideanNorm = 0;
    for(int a = 0; a < action.size(); a++) euclideanNorm += pow(action[a] - wire.action[a], 2);
    
    return (euclideanNorm * euclideanNorm) + c*(maxRewardFromWires - wire.reward) + M_E;
}
double WireFitQLearn::weightedsum(std::vector<double> &state, std::vector<Wire> &wires, std::vector<double> &action) {
    double maxRewardFromWires = -9999999;
    for(auto a = wires.begin(); a != wires.end(); ++a) if(a->reward > maxRewardFromWires) maxRewardFromWires = a->reward;
    
    double answer = 0;
    for(auto a = wires.begin(); a != wires.end(); ++a) {
        answer += a->reward / distance(state, *a, action, maxRewardFromWires);
    }
    
    return answer;
}
double WireFitQLearn::normalize(std::vector<double> &state, std::vector<Wire> &wires, std::vector<double> &action) {
    double maxRewardFromWires = -9999999;
    for(auto a = wires.begin(); a != wires.end(); ++a) if(a->reward > maxRewardFromWires) maxRewardFromWires = a->reward;
    
    double answer = 0;
    for(auto a = wires.begin(); a != wires.end(); ++a) {
        answer += 1.0 / distance(state, *a, action, maxRewardFromWires);
    }
    
    return answer;
}
