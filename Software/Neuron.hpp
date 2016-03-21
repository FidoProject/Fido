#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <cstdlib>

namespace net {

	/* Class representing a single neuron.
	 *
	 * Each neuron takes a variable number of inputs each assigned a weight and returns one output. Each neuron has a threshold value used to detemine if an output is produced (or to determine the magnitude of the neuron's output). A quality class by all accounts.
	 */
	class Neuron {
	public:
		/* Create a neuron with randomly assigned weights.
		 *
		 * Each weight is a random double between 0 and 1 
		 */
		explicit Neuron(int numInputs);

		// Create a neuron with given weights.
		explicit Neuron(std::vector<double> w);

		/* Get outputs of neuron from given inputs.	
		 *
		 * The last weight is the threshold, as documented [here](http://www.ai-junkie.com/ann/evolved/nnt6.html).
		 */
		double getOutput(std::vector<double> inputs);
		
		// Set of weights for the Neuron object.
		std::vector<double> weights;

		// Randomizes the weights of the neuron
		void randomizeWeights();

		// Return a random weight between zero and one.
		static double randomWeight() {
			return ((double)rand() / ((double)RAND_MAX / 2.0)) - 1.0;
		} 
	};
}

#endif
