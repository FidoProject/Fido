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

		void applyReinforcementToLastAction(double reward, State newState);

		void reset();
	private:
		struct History {
			State initialState, newState;
			Action action;
			double reward;

			History(State initialState_, State newState_, Action action_, double reward_) {
				initialState = initialState_;
				newState = newState_;
				action = action_;
				reward = reward_;
			}
		};

		std::vector<History> histories;


	};
};

#endif
