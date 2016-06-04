/** \file ActivationFunctions.h
 * \brief Fido's supported activation functions and their derivatives.
 *
 * An activation function is the final transformation applied to the output of a neuron. Common examples of activation functions are the sigmoid and inverse tangent functions. Derivatives are for training (ex. back propagation).
 */

#pragma once

#include <stdio.h>
#include <math.h>

namespace net {
/** The type of an activation function.
 *
 * Each activation function must take in a double and output a double.
 */
typedef double(*ActivationFunction)(double);

/** A sigmoid function.
 *
 * An "s-shaped" function that uses an activation value of 0 to return a gradient output. Similar to the hyperbolic tangent function.
 * \return between -1 and 1
 */
inline double sigmoid(double initialOutput) {
  return ( 1 / ( 1 + exp(-initialOutput / 1)));
}

/** A binary function.
 *
 * \return Either 0 or 1. 0 if input is less than 0 and 1 otherwise.
 */
inline double binary(double initialOutput) {
  return (initialOutput > 0 ? 1 : 0);
}

/** An flooring function.
 *
 * \return Floor of the input.
 */
inline double integer(double initialOutput) {
  return floor(initialOutput);
}

/** A linear function.
 *
 * \return Same as the input
 */
inline double simpleLinear(double initialOutput) {
  return initialOutput;
}

/** A tangential sigmoid or a hyperbolic tangent function.
 *
 * An "s-shaped" function that uses an activation value of 0 to return a gradient output. Similar to the sigmoid function
 * \return Same as the input
 */
inline double tanSigmoid(double initialOutput) {
  return (exp(2*initialOutput)-1) / (exp(2*initialOutput)+1);
}

//--------------------- DERIVATIVES OF ACTIVATION FUNCTIONS ----------------
/** Derivative of the sigmoid activation function.
 */
inline double sigmoidDerivative(double neuronOuput) {
  return neuronOuput * (1 - neuronOuput);
}

/** Derivative of the linear activation function.
 */
inline double simpleLinearDerivative(double neuronOutput) {
  return 1;
}

inline double tanSigmoidDerivative(double neuronOutput) {
  return 1 - pow(tanSigmoid(neuronOutput), 2);
}
}
