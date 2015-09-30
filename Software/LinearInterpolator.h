#ifndef LINEARINTERPOLATOR_H
#define LINEARINTERPOLATOR_H

#include "Interpolator.h"

namespace net {
	// A least squares wire fitted Interpolator
	class LinearInterpolator : public Interpolator {
	public:
		// Initializes a LinearInterpolator
		LinearInterpolator();

		// Initializes a LinearInterpolator using the information from a file. Returns false if the interpolator stored in the file is not a LinearInterpolator
		bool initFromFile(std::ifstream *in);

		// Stores the interpolator object in a file
		void storeInterpolator(std::ofstream *out);

		// Uses the interpolator function to compute the reward of an action vector given a set of control wires
		double getReward(const std::vector<Wire> &controlWires, const std::vector<double> &action);

		// The partial derivative of the interpolator function with respect to the reward of a control wire
		double rewardDerivative(const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires);

		// The partial derivative of the interpolator function with respect to the value of one term of the action vector of a control wire
		double actionTermDerivative(double actionTerm, double wireActionTerm, const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires);
	};
}

#endif