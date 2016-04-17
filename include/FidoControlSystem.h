#ifndef FIDOCONTROLSYSTEM_H
#define FIDOCONTROLSYSTEM_H

#include "WireFitQLearn.h"

namespace rl {
	class FidoControlSystem : public WireFitQLearn {
	public:
		/* Initializes a WireFitQLearn object with a model NN (network is copied from this), a net::Backpropagation object (used to train network),
		 * a learning rate (dictates how fast the reward values for actions change), a devaluation factor (dictates how much future rewards are valued),
		 * the dimensions of the action vectors, and how many wires the network outputs.
		 */
		FidoControlSystem(int stateDimensions, int actionDimensions, Action minAction, Action maxAction);
	};
};

#endif
