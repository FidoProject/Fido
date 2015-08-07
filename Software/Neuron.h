#ifndef NEURON_H
#define NEURON_H

#include <iostream>
#include <vector>
#include <stdlib.h>

namespace net {
	class Neuron {
	public:
		Neuron(int numInputs);
		Neuron(std::vector<double> w);
		double getOutput(std::vector<double> inputs);
		std::vector<double> weights;

		static double randomWeight() {
			return rand() % RAND_MAX;
		} 
	};
}

#endif