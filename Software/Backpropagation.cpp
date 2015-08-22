#include "Backpropagation.h"

using namespace net;

Backpropagation::Backpropagation(double learningRate_, double momentumTerm_, double targetErrorLevel_, int maximumIterations_) {
	learningRate = learningRate_;
	momentumTerm = momentumTerm_;
	targetErrorLevel = targetErrorLevel_;
	maxiumumIterations = maximumIterations_;
	hiddenActivationFunctionDerivative = sigmoidDerivative;
	outputActivationFunctionDerivative = sigmoidDerivative;
}

Backpropagation::Backpropagation() {
	hiddenActivationFunctionDerivative = sigmoidDerivative;
	outputActivationFunctionDerivative = sigmoidDerivative;
}

Backpropagation::Backpropagation(std::string filename) { 
	std::ifstream input(filename);
	initWithStream(input);
	input.close();
}

Backpropagation::Backpropagation(std::ifstream &input) {
	initWithStream(input);
}

void storeBackpropagation(std::string filename) {

}

void storeBackpropagationWithStream(std::ofstream &output) {

}

void Backpropagation::trainOnData(net::NeuralNet *network, std::vector< std::vector<double> > input, std::vector< std::vector<double> > correctOutput) {
	double totalError = 0;
	int iterations = 0;
	lastChangeInWeight.clear();
	std::vector< std::vector< std::vector<double> > > weights = network->getWeights3D();
	for(int a = 0; a < weights.size(); a++) {
		std::vector< std::vector<double> > layer;
		for(int b = 0; b < weights[a].size(); b++) {
			std::vector<double> neuron;
			for(int c = 0; c < weights[a][b].size(); c++) {
				neuron.push_back(0);
			}
			layer.push_back(neuron);
		}
		lastChangeInWeight.push_back(layer);
	}

	do {
		totalError = 0;
		for(int a = 0; a < input.size(); a++) {
			totalError += trainOnDataPoint(network, input[a], correctOutput[a]);
		}
		iterations++;
		///std::cout << "Iterations: " << iterations << "\n";
		std::cout << "Error: " << totalError << "\n";
	} while(totalError > targetErrorLevel && iterations < maxiumumIterations);
}


/// Assumes sigmoid
double Backpropagation::trainOnDataPoint(net::NeuralNet *network, std::vector<double> input, std::vector<double> correctOutput) {
	std::vector< std::vector<double> > outputs = network->feedForward(input);
	std::vector< std::vector< std::vector<double> > > weights = network->getWeights3D();
	std::vector< std::vector<double> > errors;
	double networkError = 0;

	/// Compute output layer error
	std::vector<double> outputNeuronErrors;
	std::vector<double> outputLayerOutput = outputs[outputs.size() - 1];
	for(int neuronIndex = 0; neuronIndex < outputLayerOutput.size(); neuronIndex++) {
		double outputNeuronError = (correctOutput[neuronIndex] - outputLayerOutput[neuronIndex]) * outputActivationFunctionDerivative(outputLayerOutput[neuronIndex]);
		networkError += pow(correctOutput[neuronIndex] - outputLayerOutput[neuronIndex], 2);
		outputNeuronErrors.push_back(outputNeuronError);
	}
	errors.push_back(outputNeuronErrors);

	/// Compute hidden layer error
	for(int hiddenLayerIndex = network->numHiddenLayers - 1; hiddenLayerIndex >= 0; hiddenLayerIndex--) {
		std::vector<double> currentLayerError;
		std::vector<double> currentHiddenLayerOutput = outputs[hiddenLayerIndex];
		std::vector<double> lastLayerError = errors[errors.size() - 1];
		std::vector< std::vector<double> > lastLayerWeights = weights[hiddenLayerIndex + 1];
		
		for(int neuronIndex = 0; neuronIndex < network->numNeuronsPerHiddenLayer; neuronIndex++) {
			double errorsTimesWeights = 0;
			for(int previousNeuronIndex = 0; previousNeuronIndex < lastLayerError.size(); previousNeuronIndex++) {
				errorsTimesWeights += lastLayerError[previousNeuronIndex] * lastLayerWeights[previousNeuronIndex][neuronIndex];
			}
			double hiddenNeuronError = hiddenActivationFunctionDerivative(currentHiddenLayerOutput[neuronIndex]) * errorsTimesWeights;
			currentLayerError.push_back(hiddenNeuronError);
		}
		errors.push_back(currentLayerError);
	}

	/// Update weights
	for(int errorIndex = 0; errorIndex < errors.size(); errorIndex++) {
		int layerIndex = (errors.size() - 1) - errorIndex;
		for(int neuronIndex = 0; neuronIndex < errors[errorIndex].size(); neuronIndex++) {
			if(errorIndex == errors.size() - 1) {
				for(int inputIndex = 0; inputIndex < input.size(); inputIndex++) {
					double deltaWeight = learningRate * errors[errorIndex][neuronIndex] * input[inputIndex] + lastChangeInWeight[layerIndex][neuronIndex][inputIndex]*momentumTerm;
					weights[layerIndex][neuronIndex][inputIndex] += deltaWeight;
					lastChangeInWeight[layerIndex][neuronIndex][inputIndex] = deltaWeight;
				}
			} else {
				for(int previousOutput = 0; previousOutput < outputs[layerIndex - 1].size(); previousOutput++) {
					double deltaWeight = learningRate * errors[errorIndex][neuronIndex] * outputs[layerIndex - 1][previousOutput] + lastChangeInWeight[layerIndex][neuronIndex][previousOutput]*momentumTerm;
					weights[layerIndex][neuronIndex][previousOutput] += deltaWeight;
					lastChangeInWeight[layerIndex][neuronIndex][previousOutput] = deltaWeight;
				}
			}

			/// Change the activation weight
			double deltaWeight = -learningRate * errors[errorIndex][neuronIndex] + lastChangeInWeight[layerIndex][neuronIndex][weights[layerIndex][neuronIndex].size() - 1]*momentumTerm;
			weights[layerIndex][neuronIndex][weights[layerIndex][neuronIndex].size() - 1] += deltaWeight;
			lastChangeInWeight[layerIndex][neuronIndex][weights[layerIndex][neuronIndex].size() - 1] = deltaWeight;
		}
	}

	network->setWeights3D(weights);

	return networkError;
}

std::map<std::string, ActivationFunction> Backpropagation::getDerivedActivationFunctionNames() {
	std::map<std::string, ActivationFunction> map;
	map["sigmoidDerivative"] = Backpropagation::sigmoidDerivative;
	map["simpleLinearDerivative"] = Backpropagation::simpleLinearDerivative;

	return map;
}

void Backpropagation::setDerivedHiddenActivationFunction(std::string name) {
	hiddenActivationFunctionDerivative = getDerivedActivationFunctionNames()[name];
}

void Backpropagation::setDerivedOutputActivationFunction(std::string name) {
	outputActivationFunctionDerivative = getDerivedActivationFunctionNames()[name];
}

std::string Backpropagation::getDerivedHiddenActivationFunctionName() {
	std::map<std::string, ActivationFunction> nameMap = getDerivedActivationFunctionNames();
	for(auto a = nameMap.begin(); a != nameMap.end(); ++a) if(a->second == hiddenActivationFunctionDerivative) return a->first;
}

std::string Backpropagation::getDerivedOutputActivationFunctionName() {
	std::map<std::string, ActivationFunction> nameMap = getDerivedActivationFunctionNames();
	for(auto a = nameMap.begin(); a != nameMap.end(); ++a) if(a->second == outputActivationFunctionDerivative) return a->first;
}

void Backpropagation::initWithStream(std::ifstream &input) {
	if(input.is_open()) {
		input >> learningRate >> momentumTerm >> targetErrorLevel >> maxiumumIterations;

		std::string hiddenActivationFunctionDerivativeName, outputActivationFunctionDerivativeName;
		input >> hiddenActivationFunctionDerivativeName >> outputActivationFunctionDerivativeName;
		setDerivedHiddenActivationFunction(hiddenActivationFunctionDerivativeName);
		setDerivedOutputActivationFunction(outputActivationFunctionDerivativeName);

	} else {
		std::cout << "Could not retrieve neural network from file\n";
		throw 1;
	}
}