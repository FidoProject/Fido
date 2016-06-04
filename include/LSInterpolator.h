#pragma once

#include "Interpolator.h"

namespace rl {
/**
 * \brief A least squares wire fitted Interpolator
 */
class LSInterpolator : public Interpolator {
public:
  double smoothingFactor, e;

  /**
   * \brief Initializes a LSInterpolator with default parameters
   *
   * The default smoothingFactor is 0, while the default e is 0.01.
   */
  LSInterpolator();

  /**
   * \brief Initializes a LSInterpolator with custom parameters
   *
   * \param smoothingFactor_ the smoothing factor for the interpolator
   * \param e_ a small constant (less than 1)
   */
  LSInterpolator(double smoothingFactor_, double e_);

  /**
   * \brief Initializes a LSInterpolator using the information from a file.
   *
   * \param in the file from which initialize the LSInterpolator
   *
   * \return False if the interpolator stored in the file is not a LSInterpolator.
   */
  bool initFromStream(std::ifstream *in);

  /**
   * \brief Stores the interpolator object in a file
   *
   * \param out the file to which to write the LSInterpolator
   */
  void store(std::ofstream *out);

  /**
   * \brief Uses the interpolator function to compute the reward of an action vector given a set of control wires
   *
   * \param controlWires the input vector of control wires
   * \param action the action vector for which the reward is computed
   *
   * \return the expected reward of the action vector
   */
  double getReward(const std::vector<Wire> &controlWires, const std::vector<double> &action);

  /**
   * \brief Computes the partial derivative of the interpolator function with respect to the reward of a control wire
   *
   * \param action the action vector
   * \param wire the wire
   * \param controlWires the control wires
   *
   * \return the reward derivative
   */
  double rewardDerivative(const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires);

  /**
   * \brief Computes the partial derivative of the interpolator function with respect to the value of one term of the action vector of a control wire
   *
   * \param actionTerm the action term
   * \param wireActionTerm the wire action term
   * \param action the action vector
   * \param wire the wire
   * \param controlWires the vector of control wires
   *
   * \return the action term derivative
   */
  double actionTermDerivative(double actionTerm, double wireActionTerm, const std::vector<double> &action, const Wire &wire, const std::vector<Wire> &controlWires);
private:
  /**
   * \brief Computes a weighted distance between the action vector of the wire and an action vector
   *
   * \param wire the wire in the distance computation
   * \param action the action vector in the distance computation
   * \param maxReward the given maximum reward
   */
  double distanceBetweenWireAndAction(const Wire &wire, const std::vector<double> &action, double maxReward);

  /**
   * \brief Computes the weighted sum of all the distances between the wires and the action vector
   *
   * \param wires the wires in the distance computation
   * \param action the action vector in the distance computation
   * \param maxReward the given maximum reward
   */
  double weightedSum(const std::vector<Wire> &wires, const std::vector<double> &action, double maxReward);

  /**
   * \brief Used in the wire interpolator function and its derivatives for normalization
   */
  double normalize(const std::vector<Wire> &wires, const std::vector<double> &action, double maxReward);
};
}
