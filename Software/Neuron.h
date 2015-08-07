#ifndef NEURON_H
#define NEURON_H

#include <iostream>
#include <vector>
#include <stdlib.h>

namespace net {

	/* Class representing a single neuron.
	 *
	 * Includes inputs, outputs, weights, and a threshold.  A quality class by all accounts.
	 * */
	class Neuron {
	public:
		/* Create a neuron with randomly assigned weights.
		 *
		 * Each weight is a random double between 0 and 1 */
		Neuron(int numInputs);

		// Create a neuron with given weights.
		Neuron(std::vector<double> w);

		/* Get outputs of neuron from given inputs.	
		 *
		 * The last weight is the threshold, as documented [here](http://www.ai-junkie.com/ann/evolved/nnt6.html). */
		double getOutput(std::vector<double> inputs);
		
		// Set of weights for the Neuron object.
		std::vector<double> weights;

		// Return a random weight between zero and one.
		static double randomWeight() {
			return rand() % RAND_MAX;
		} 
	};
}

#endif