/**
 * \mainpage
 * 
 * \section welcome Welcome
 * Welcome to the official Fido documentation. Here you will find a detailed view of all the Fido classes and functions. 
 *
 * \section example Short Example
 * Here is a short, simple example of Fido's functionality. In this example, we will train a neural network to perform linear regression.
 *
 * \code
 * #include "NeuralNet.h"
 * #include "Backpropagation.h"
 * #include <iostream>
 *
 * int main() {
 *   // Creates a neural network with
 *   // 1 input, 1 output, 2 hidden layers, 4 neurons per hidden layer,
 *   // a sigmoid activation function for hidden layers, and a linear activation function on the final layer.
 *   net::NeuralNet neuralNetwork = net::NeuralNet(1, 1, 2, 4, "sigmoid");
 *   neuralNetwork.setOutputActivationFunction("simpleLinear");
 *
 *   std::vector< std::vector<double> > input = { {1}, {2}, {5}, {6} };
 *   std::vector< std::vector<double> > correctOutput = { {2}, {4}, {10}, {12} };
 *
 *   // Create backpropagation object with
 *   // a learning rate of 10%, a momentum term of 0.001, an acceptable error level of 0.1%,
 *   // and a maximum number of training iterations of 10000
 *   net::Backpropagation backprop = net::Backpropagation(0.1, 0.001, 0.001, 10000);
 *   backprop.trainOnData(&neuralNetwork, input, correctOutput);
 *
 *   // Cycle through inputs and print the outputs
 *   for (std::vector<double> current: input) {
 *       std::cout << "Input: " << current[0] << "\tNeural network's answer:" << neuralNetwork.getOutput(current)[0] << std::endl;
 *   }
 * }
 * \endcode
 */