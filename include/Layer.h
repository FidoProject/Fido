#pragma once

#include <map>
#include <fstream>
#include <string>
#include <iostream>

#include "Neuron.h"
#include "ActivationFunctions.h"

namespace net {
/**
 * \brief A layer in a NeuralNet.
 */
class Layer {
public:
  /**
   * \brief The activation function for the neurons of this layer
   *
   * An activation function takes the summation of the inputs times the weights of a neuron and uses an activation value of 0 to filter the output
   */
  ActivationFunction activationFunction;

  /**
   * \brief Holds the neurons of this layer
   */
  std::vector<Neuron> neurons;

  /**
   * \brief Initialize a layer with the default sigmoid activation function.
   */
  Layer();

  /**
   * \brief Initialize a layer from a saved text file
   *
   * The saved text file must be formatted as outputted
   * by the storeNetWithStream(std::ofstream *output) function.
   *
   * \param input the text file for the Layer object to be initialized from
   */
  explicit Layer(std::ifstream *input);

  /**
   * \brief Sets the activation function to the given string
   *
   * \param name the name of the activation function
   */
  void setActivationFunctionWithName(std::string name);

  /**
   * \brief Gets the name of the layer's activation function
   */
  std::string getActivationFunctionName();

  /**
   * \brief Store the layer to a text file
   *
   * This can later be read by the Layer(std::ifstream *input) constructor.
   *
   * \param output the text file for the layer object to be written to
   */
  void store(std::ofstream *output);

  /**
   * \brief Gets the map of activation function name to ActivationFunction
   */
  static std::map<std::string, ActivationFunction> getActivationFunctionNameMap() {
    std::map<std::string, ActivationFunction> map;
    map["sigmoid"] = sigmoid;
    map["binary"] = binary;
    map["integer"] = integer;
    map["simpleLinear"] = simpleLinear;
    map["tanSigmoid"] = tanSigmoid;

    return map;
  }
  /**
   * \brief Gets the map of activation function name to a derived ActivationFunction
   */
  static std::map<std::string, ActivationFunction> getDerivedActivationFunctionNames() {
    std::map<std::string, ActivationFunction> map;
    map["sigmoid"] = sigmoidDerivative;
    map["simpleLinear"] = simpleLinearDerivative;
    map["tanSigmoid"] = tanSigmoidDerivative;

    return map;
  }

};
}
