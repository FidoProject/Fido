#include "Backpropagation.h"

Backpropogation::Backpropogation(double learningRate_, double momentumTerm_, double targetErrorLevel_, int maximumIterations_) {
	learningRate = learningRate_;
	momentumTerm = momentumTerm_;
	targetErrorLevel = targetErrorLevel_;
	maxiumumIterations = maximumIterations_;
}

void Backpropogation::trainOnData(net::NeuralNet *network, std::vector< std::vector<double> > input, std::vector< std::vector<double> > correctOutput) {
	double totalError = 0;
	int iterations = 0;
	do {
		for(int a = 0; a < input.size(); a++) {
			totalError += trainOnDataPoint(network, input[a], correctOutput[a]);
		}
		iterations++;
	} while(totalError > targetErrorLevel && iterations < maxiumumIterations);
}


/// Implement momentum term
/// Remember to update the activation energy weight
/// Assumes sigmoid
double Backpropogation::trainOnDataPoint(net::NeuralNet *network, std::vector<double> input, std::vector<double> correctOutput) {
	std::vector< std::vector<double> > outputs = network->feedForward(input);
	std::vector< std::vector< std::vector<double> > > weights = network->getWeights3D();
	std::vector< std::vector<double> > errors;
	double networkError = 0;

	/// Compute output layer error
	std::vector<double> outputNeuronErrors;
	std::vector<double> outputLayerOutput = outputs[outputs.size() - 1];
	for(int neuronIndex = 0; neuronIndex < outputLayerOutput.size(); neuronIndex++) {
		double outputNeuronError = (correctOutput[neuronIndex] - outputLayerOutput[neuronIndex]) * outputLayerOutput[neuronIndex] * (1 - outputLayerOutput[neuronIndex]);
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
			for(int previousNeuronIndex = 0; previousNeuronIndex < lastLayerWeights.size(); previousNeuronIndex++) {
				errorsTimesWeights += lastLayerError[previousNeuronIndex] * lastLayerWeights[previousNeuronIndex][neuronIndex];
			}
			double hiddenNeuronError = currentHiddenLayerOutput[neuronIndex] * (1 - currentHiddenLayerOutput[neuronIndex]) * errorsTimesWeights;
			currentLayerError.push_back(hiddenNeuronError);
		}
		errors.push_back(currentLayerError);
	}

	/// Update weights
	for(int layerIndex = 0; layerIndex < errors.size(); layerIndex++) {
		for(int neuronIndex = 0; neuronIndex < errors[layerIndex].size(); neuronIndex++) {
			for(int previousOutput = 0; previousOutput < outputs[layerIndex + 1].size(); previousOutput++) {
				weights[layerIndex][neuronIndex][previousOutput] += learningRate * errors[layerIndex][neuronIndex] * outputs[layerIndex + 1][previousOutput];
			}
		}
	}

	network->setWeights3D(weights);

	return networkError;
}