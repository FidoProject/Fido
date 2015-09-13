#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

namespace net {
	struct Wire {
	    std::vector<double> action;
	    double reward;
	};

	class Interpolator {
	public:
		// Uses the interpolator function to compute the reward of an action vector given a set of control wires
		virtual double getReward(const std::vector<Wire> &controlWires, const std::vector<double> &action) = 0;

		// The partial derivative of the interpolator function with respect to the reward of a control wire
        virtual double rewardDerivative(const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires) = 0;

        // The partial derivative of the interpolator function with respect to the value of one term of the action vector of a control wire
        virtual double actionTermDerivative(double actionTerm, double wireActionTerm, const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires) = 0;
	};
}

#endif