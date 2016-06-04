#pragma once

#include <fstream>
#include <map>

#include "Layer.h"
#include "ActivationFunctions.h"

namespace net {

/**
 * \brief A neural network.
 *
 * A flexible implementation designed to be usable in a wide scope of projects.
 */
class NeuralNet {
public:
  /**
   * \brief Constructs an empty neural network
   */
  NeuralNet();

  /**
   * \brief Manually initializes a neural network
   *
   * \param numInputs the number of inputs to the network (or neurons in the input layer)
   * \param numOutputs the number of outputs to the network (or neurons in the output layer)
   * \param numHiddenLayers the number of hidden layers
   * \param numNeuronsPerHiddenLayer the number of neurons per hidden layer.
   * \param activationFunctionName the name of the activation function to be used in the NN
   */
  NeuralNet(unsigned int numInputs, unsigned int numOutputs, unsigned int numHiddenLayers, unsigned int numNeuronsPerHiddenLayer, std::string activationFunctionName);

  /**
   * \brief Constructs a neural network copying another networks architecture with random weights.
   *
   * The new network has the same number of inputs, outputs, hidden layers, and neurons per hidden layer as the network given.
   *
   * \param otherNet the network to be copied
   */
  explicit NeuralNet(NeuralNet* otherNet);

  /**
   * \brief Restores a neural network from the specified file
   *
   * \param input the file the network is to be constructed from
   */
  explicit NeuralNet(std::ifstream *input);

  /**
   * \brief Stores a neural network in the specified file
   *
   * Stores a neural network using ofstream. Useful for appending a
   * network to the end of a file without overwriting it. WARNING:
   * will not close ofstream.
   *
   * \param output the file the network is to be written to
   */
  void store(std::ofstream *output);

  /**
   * \brief Gets the weights of each neuron in the net.
   *
   * \return A 1d vector of neuron weights starting from the first neuron
   * in the first layer to the last neuron in the output layer.
   */
  std::vector<double> getWeights();

  /**
   * \brief Gets the weights of each neuron in the net
   *
   * \return A 3d vector of neuron weights.
   */
  std::vector< std::vector< std::vector<double> > > getWeights3D();

  /**
   * \brief Sets the weights of each input for each neuron in the net
   *
   * \param w the vector of weights from which neuron weights are set,
   * starting from the first neuron in the first layer to the last
   * neuron in the output layer
   */
  void setWeights(std::vector<double> w);

  /**
   * \brief Sets the weights of each input for each neuron in the net
   *
   * \param w the 3d vector of weights from which neuron weights are set
   */
  void setWeights3D(std::vector< std::vector< std::vector<double> > > w);

  /**
   * \brief Randomizes all the weights of the neurons in the net
   */
  void randomizeWeights();

  /**
   * \brief Gets the outputs of the network for a set of inputs
   *
   * The crowning function of this class ;)
   *
   * \param input the inputs to the neural network
   * \return the outputs of the neural network
   */
  std::vector<double> getOutput(std::vector<double> input);

  /**
   * \brief Gets the output of each layer of neurons as an input is fed for just as it is fed forward in getOutput
   *
   * \param input the inputs to the neural network
   * \return a 2d vector containing the outputs of each layer
   */
  std::vector< std::vector<double> > feedForward(std::vector<double> input);

  /**
   * \brief Computes the error gradients of each layer in the NeuralNet
   *
   * \param input the input to the neural net
   * \param correctOutput what the neural net would ideally output when fed the provided input
   * \return a 2d vector containing the gradients of each layer starting from the first layer and ending with the last
   */
  std::vector< std::vector< std::vector<double> > > getGradients(const std::vector<double> &input, const std::vector<double> &correctOutput);

  /**
   * \brief Prints the weights of the neurons of the layers of the net
   */
  void printWeights();

  /**
   * \brief Gets the number of hidden layers in the neural net
   *
   * \return the number of layers minus 1
   */
  unsigned int numberOfHiddenLayers();

  /**
   * \brief Gets the number of input neurons in the NeuralNet
   *
   * \return the number of inputs to the network
   */
  unsigned int numberOfInputs();

  /**
   * \brief Gets the number of output neurons in the NeuralNet
   *
   * \return the number of outputs to the network
   */
  unsigned int numberOfOutputs();

  /**
   * \brief Gets the number of hidden neurons in the NeuralNet
   *
   * \return the number of hidden neurons in the network
   */
  unsigned int numberOfHiddenNeurons();

  /**
   * \brief Sets the activation function of the output layer
   *
   * \param name the name of the activation function
   */
  void setOutputActivationFunction(std::string name);

  /**
   * \brief Gets the name of the activation function used for hidden layers
   *
   * \return the string representing the activation function
   */
  std::string getHiddenActivationFunctionName();

  /**
   * \brief Gets the name of the activation function used for output layers
   *
   * \return the string representing the activation function
   */
  std::string getOutputActivationFunctionName();

  /**
   * \brief Removes a neuron from the network. Deletes all weights associated with the deleted neuron.
   *
   * \return hiddenLayerIndex the index of the layer of the neuron to be deleted; 0 is input layer
   * \return neuronIndex the index of the neuron; same as the neuron's index in the neurons vector of its layer
   */
  void removeNeuron(unsigned int hiddenLayerIndex, unsigned int neuronIndex);

  /**
   * \brief A two dimensional network of neurons.
   */
  std::vector< Layer > net;
};
}
