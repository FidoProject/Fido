#include "NeuralNet.h"

using namespace net;

NeuralNet::NeuralNet() {
	numInputs = 0;
	numOutputs = 0;
	numHiddenLayers = 0;
	numNeuronsPerHiddenLayer = 0;
}

NeuralNet::NeuralNet(int numInputs_, int numOutputs_, int numHiddenLayers_, int numNeuronsPerHiddenLayer_, double(*activationFunction_)(double initialOutput)) {
	numInputs = numInputs_;
	numOutputs = numOutputs_;
	numHiddenLayers = numHiddenLayers_;
	numNeuronsPerHiddenLayer = numNeuronsPerHiddenLayer_;
	activationFunction = activationFunction_;

    setupNeuronLayers();
}

NeuralNet::NeuralNet(NeuralNet* otherNet) {
	numInputs = otherNet->numInputs;
	numOutputs = otherNet->numOutputs;
	numHiddenLayers = otherNet->numHiddenLayers;
	numNeuronsPerHiddenLayer = otherNet->numNeuronsPerHiddenLayer;
	activationFunction = otherNet->activationFunction;

	setupNeuronLayers();
}

NeuralNet::NeuralNet(std::string filename, double(*activationFunction_)(double initialOutput)) {
	activationFunction = activationFunction_;
    
	std::ifstream input(filename);
    if(input.is_open()) { 
        input >> numInputs >> numOutputs >> numHiddenLayers >> numNeuronsPerHiddenLayer;
        
        setupNeuronLayers();
        
        std::vector<double> newWeights;
        double middleMan = 0;
        while(input >> middleMan) newWeights.push_back(middleMan);
        setWeights(newWeights);
        
        input.close();
    } else {
        std::cout << "Could not retrieve neural network from file\n";
        throw 1;
    }
}

void NeuralNet::storeNet(std::string filename) {
    std::ofstream output(filename);
    if(output.is_open()) {
        output << numInputs << " " << numOutputs << " " << numHiddenLayers << " " << numNeuronsPerHiddenLayer << "\n";
        std::vector<double> weights = getWeights();
        for(int a = 0; a < weights.size(); a++) output << weights[a] << " ";

        output.close();
    } else {
        std::cout << "Could not store neural network\n";
        throw 1;
    }
}

std::vector<double> NeuralNet::getWeights() {
	std::vector<double> weights;
	for(int a = 0; a < net.size(); a++) {
		for(int b = 0; b < net[a].size(); b++) {
			for(int c = 0; c < net[a][b].weights.size(); c++) {
				weights.push_back(net[a][b].weights[c]);
			}
		}
	}
	return weights;
}

std::vector< std::vector< std::vector<double> > > NeuralNet::getWeights3D() {
	std::vector< std::vector< std::vector<double> > > weights;
	for(int a = 0; a < net.size(); a++) {
		std::vector< std::vector<double> > layer;
		for(int b = 0; b < net[a].size(); b++) {
			layer.push_back(net[a][b].weights);
		}
		weights.push_back(layer);
	}
	return weights;
}

std::vector< std::vector<double> > NeuralNet::getHiddenLayerWeights2D(int hiddenLayerIndex) {
	if(hiddenLayerIndex < 0 || hiddenLayerIndex > numHiddenLayers-1) {
		std::cout << "Invalid hidden layer index\n";
		throw 1;
	}
	std::vector< std::vector<double> > weights;
	for(int b = 0; b < net[hiddenLayerIndex].size(); b++) {
		weights.push_back(net[hiddenLayerIndex][b].weights);
	}
	return weights;
}

std::vector< std::vector<double> > NeuralNet::getOutputLayerWeights2D() {
	std::vector< std::vector<double> > weights;
	for(int b = 0; b < net[net.size()-1].size(); b++) {
		weights.push_back(net[net.size() - 1][b].weights);
	}
	return weights;
}

void NeuralNet::setWeights(std::vector<double> w) {
	int counter = 0;
	for(int a = 0; a < net.size(); a++) {
		for(int b = 0; b < net[a].size(); b++) {
			for(int c = 0; c < net[a][b].weights.size(); c++) {
				net[a][b].weights[c] = w[counter];
				counter++;
			}
		}
	}
	if(counter != w.size()) {
		std::cout << "Not the right number of weights\n";
		throw 1;
	}
}

void NeuralNet::setWeights3D(std::vector< std::vector< std::vector<double> > > w) {
	for(int a = 0; a < net.size(); a++) {
		for(int b = 0; b < net[a].size(); b++) {
			net[a][b].weights = w[a][b];
		}
	}
}

void NeuralNet::setHiddenLayerWeights2D(int hiddenLayerIndex, std::vector< std::vector<double> > w) {
	for(int b = 0; b < net[hiddenLayerIndex].size(); b++) {
		for(int c = 0; c < net[hiddenLayerIndex][b].weights.size(); c++) {
			net[hiddenLayerIndex][b].weights[c] = w[b][c];
		}
	}
}

void NeuralNet::setOutputLayerWeights2D(std::vector< std::vector<double> > w) {
	for(int b = 0; b < net[net.size() - 1].size(); b++) {
		for(int c = 0; c < net[net.size() - 1][b].weights.size(); c++) {
			net[net.size() - 1][b].weights[c] = w[b][c];
		}
	}
}

std::vector<double> NeuralNet::getOutput(std::vector<double> input) {
	std::vector<double> output;
	for(int a = 0; a < net.size(); a++) {
		if(a > 0) input = output;
		output.clear();
		for(int b = 0; b < net[a].size(); b++) {
            double out = activationFunction(net[a][b].getOutput(input));
			output.push_back(out);
		}
	}
	return output;
}

std::vector< std::vector<double> > NeuralNet::feedForward(std::vector<double> input) {
	std::vector< std::vector<double> > output;
	for(int a = 0; a < net.size(); a++) {
		if(a > 0) input = output[output.size() - 1];

		std::vector<double> outputLayer;

		for(int b = 0; b < net[a].size(); b++) {
			double out = activationFunction(net[a][b].getOutput(input));
			outputLayer.push_back(out);
		}

		output.push_back(outputLayer);
	}
	return output;
}

void NeuralNet::setupNeuronLayers() {
    std::vector<Neuron> firstHiddenLayer;
    for(int a = 0; a < numNeuronsPerHiddenLayer; a++) firstHiddenLayer.push_back(Neuron(numInputs));
    net.push_back(firstHiddenLayer);
    
    for(int a = 0; a < numHiddenLayers; a++) {
        std::vector<Neuron> hiddenLayer;
        for(int b = 0; b < numNeuronsPerHiddenLayer; b++) hiddenLayer.push_back(Neuron(numNeuronsPerHiddenLayer));
        net.push_back(hiddenLayer);
    }
    
    std::vector<Neuron> outputLayer;
    for(int a = 0; a < numOutputs; a++) outputLayer.push_back(Neuron(numNeuronsPerHiddenLayer));
    net.push_back(outputLayer);
}