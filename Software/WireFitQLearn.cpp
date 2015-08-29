#include "WireFitQLearn.h"

using namespace net;

static const double smoothingFactor = 0.2;

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
    std::vector<Wire> controlWires = getWires(lastState);
    double scalingFactor = scalingFactorToMillis * elapsedTimeMillis;
    
    /// Update Q value according to adaptive learning
    double oldRewardForLastAction = getRewardUsingInterpolator(controlWires, lastAction);
    double feedback = ((1/scalingFactor)*(reward + (pow(devaluationFactor, scalingFactor)*highestReward(newState)))) + (1 - 1/scalingFactor) * highestReward(lastState);
    double newRewardForLastAction = ((1 - learningRate) * oldRewardForLastAction) + (learningRate*feedback);
    
    /// Generate new control wires using newReward and lastAction
    
    /// Train network using new control wires
    
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

std::vector<Wire> WireFitQLearn::newControlWires(double newReward, const std::vector<double> &action, std::vector<Wire> controlWires) {
    double errorTarget = 0.00001;
    int maxIterations = 10000;
    
    double error = 0;
    int iterations = 0;
    
    do {
        // do stuffs
        
        
        error = pow(newReward - getRewardUsingInterpolator(controlWires, action), 2);
        iterations++;
    } while(error > errorTarget && iterations < maxIterations);
    
    return controlWires;
}

double WireFitQLearn::rewardDerivative(const std::vector<double> &action, const Wire &wire, const std::vector<Wire> controlWires) {
    double norm = normalize(controlWires, action);
    double wsum = weightedSum(controlWires, action);
    
    double maxRewardFromWires = -9999999;
    for(auto a = controlWires.begin(); a != controlWires.end(); ++a) if(a->reward > maxRewardFromWires) maxRewardFromWires = a->reward;
    double distance = distanceBetweenWireAndAction(wire, action, maxRewardFromWires);
    
    return (norm * (distance + wire.reward*smoothingFactor) - wsum*smoothingFactor) / pow(norm * distance, 2);
}

std::vector<double> WireFitQLearn::actionDerivative(const std::vector<double> &action, Wire wire, const std::vector<Wire> controlWires) {
    double norm = normalize(controlWires, action);
    double wsum = weightedSum(controlWires, action);
    double maxRewardFromWires = -9999999;
    for(auto a = controlWires.begin(); a != controlWires.end(); ++a) if(a->reward > maxRewardFromWires) maxRewardFromWires = a->reward;
    double distance = distanceBetweenWireAndAction(wire, action, maxRewardFromWires);
    
    /// Multiply the wire's action by a scalar term
    std::transform(wire.action.begin(), wire.action.end(), wire.action.begin(), std::bind1st( std::multiplies<double>(), (wsum - norm*wire.reward)*2 ));
    
    /// Divide the wire's action by a scalar term
    std::transform(wire.action.begin(), wire.action.end(), wire.action.begin(), std::bind1st( std::divides<double>(), pow(norm*distance, 2) ));
    
    return wire.action;
}

double WireFitQLearn::getRewardUsingInterpolator(const std::vector<Wire> &controlWires, const std::vector<double> &action) {
    return weightedSum(controlWires, action) / normalize(controlWires, action);
}

double WireFitQLearn::distanceBetweenWireAndAction(const Wire &wire, const std::vector<double> &action, double maxReward) {
    double e = 0.01;
    
    double euclideanNorm = 0;
    for(int a = 0; a < action.size(); a++) euclideanNorm += pow(action[a] - wire.action[a], 2);
    
    return (euclideanNorm * euclideanNorm) + smoothingFactor*(maxReward - wire.reward) + e;
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
