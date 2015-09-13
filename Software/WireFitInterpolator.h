#ifndef WIREFITINTERPOLATOR_H
#define WIREFITINTERPOLATOR_H

#include <string>

class Interpolator;

namespace net {

	class WireFitInterpolator : public Interpolator {
	public:
		double smoothingFactor, e;

		// Initializes a WireFitInterpolator with default default parameters
		WireFitInterpolator();

		// Initializes a WireFitInterpolator with custom parameters
		WireFitInterpolator(double smoothingFactor_, double e_);

		// Initializes a WireFitInterpolator using the information from a file. Throws an exception if the interpolator stored in the file is not a WireFitInterpolator
		void initFromFile(std::ifstream *in);

		// Stores the interpolator object in a file
		void storeInterpolator(std::ofstream *out);

		// Uses the interpolator function to compute the reward of an action vector given a set of control wires
		double getReward(const std::vector<Wire> &controlWires, const std::vector<double> &action);

		// The partial derivative of the interpolator function with respect to the reward of a control wire
        double rewardDerivative(const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires);

        // The partial derivative of the interpolator function with respect to the value of one term of the action vector of a control wire
        double actionTermDerivative(double actionTerm, double wireActionTerm, const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires);
	private:
		// Computes a weighted distance between the action vector of the wire and an action vector
        double distanceBetweenWireAndAction(const Wire &wire, const std::vector<double> &action, double maxReward);

        // Computes the weighted sum of all the distances between the wires and the action vector 
        double weightedSum(const std::vector<Wire> &wires, const std::vector<double> &action, double maxReward);

        // Used in the wire interpolator function and its derivatives for normalization
        double normalize(const std::vector<Wire> &wires, const std::vector<double> &action, double maxReward);
	};
}

#endif