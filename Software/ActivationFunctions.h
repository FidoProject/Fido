#ifndef ACTIVATIONFUNCTIONS_H
#define ACTIVATIONFUNCTIONS_H

#include <stdio.h>
#include <math.h>

namespace net {
	// Defines the type for an activation function for the neurons of a neural net
	typedef double(*ActivationFunction)(double);

	/// ACTIVATION FUNCTIONS
	// An example of a common "s-shaped" function that takes the initial output of a neuron and uses an activation value of 0 to return a gradient output
	static double sigmoid(double initialOutput) {
		return ( 1 / ( 1 + exp(-initialOutput / 1)));
	}

	// An example of a common function that takes the initial output of a neuron and uses an activation value of 0 to return a binary output
	static double binary(double initialOutput) {
		return (initialOutput > 0 ? 1 : 0);
	}

	// An example of a common function that takes the initial output of a neuron and uses an activation value of 0 to return a binary output
	static double integer(double initialOutput) {
		return floor(initialOutput);
	}

	// Takes the initial output of a neuron and returns it unchanged
	static double simpleLinear(double initialOutput) {
		return initialOutput;
	}

	// Hyperbolic tangent (tanh) activation function
	static double tanSigmoid(double initialOutput) {
		return (1 - exp(-2*initialOutput)) / (1 + exp(-2*initialOutput));
	}

	/// DERIVATIVES OF ACTIVATION FUNCTIONS
	// Derivative of the sigmoid activation function
	static double sigmoidDerivative(double neuronOuput) {
		return neuronOuput * (1 - neuronOuput);
	}

	// Derivative of the simple linear activation function
	static double simpleLinearDerivative(double neuronOutput) {
		return 1;
	}
}

#endif
