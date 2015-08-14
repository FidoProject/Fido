#ifndef QLEARN_H
#define QLEARN_H

#include <iostream>
#include <vector>
#include <stdlib.h>

#include "NeuralNet.h"
#include "BackPropagation.h"

namespace net {
	class QLearn {
	public:
		NeuralNet *network;
		Backpropogation backprop;
		int lastAction;
		double learningRate, devaluationFactor;
		std::vector<double> lastState;

		QLearn(NeuralNet *network_, Backpropogation backprop_, double learningRate_, double devaluationFactor_);
		int maxRewardForState(std::vector<double> state);
		int getAction(std::vector<double> currentState);
		void applyReinforcement(int reward, std::vector<double> newState);
	};
};

#endif