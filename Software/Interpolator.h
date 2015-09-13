#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <iostream>
#include <vector>
#include <fstream>

#include "WireFitInterpolator.h"

namespace net {
	struct Wire {
	    std::vector<double> action;
	    double reward;
	};

	class Interpolator {
	public:

		// Initializes the parameters of an interpolator from a file. Throws an exception if the interpolator stored in the file is not the correct type
		virtual void initFromFile(std::ifstream *in) = 0;

		// Stores the parameters of an interpolator in a file
		virtual void storeInterpolator(std::ofstream *out) = 0;

		// Uses the interpolator function to compute the reward of an action vector given a set of control wires
		virtual double getReward(const std::vector<Wire> &controlWires, const std::vector<double> &action) = 0;

		// The partial derivative of the interpolator function with respect to the reward of a control wire
        virtual double rewardDerivative(const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires) = 0;

        // The partial derivative of the interpolator function with respect to the value of one term of the action vector of a control wire
        virtual double actionTermDerivative(double actionTerm, double wireActionTerm, const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires) = 0;

		static Interpolator * getAnyInterpolatorFromFile(std::ifstream *in) {
			int placeInFile = in->tellg();
			Interpolator *interpolator;

			interpolator = new WireFitInterpolator();
			interpolator->initFromFile(in);
		};

	};
}

#endif