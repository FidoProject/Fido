# Fido

![build status](https://travis-ci.org/FidoProject/Fido.svg?branch=master)

An open-source, highly modular C++ machine learning library for embedded electronics and robotics. Fido includes implementations of neural networks, reinforcement learning, and genetic algorithms. Fido is especially suited for robotic contexts, as it is written in C++ with minimal use of the standard library, comes packaged with a robotic simulator, and provides and easy interface in which to write robotic drivers.

## Getting Started

Clone the github repository.
```
$ git clone https://github.com/FidoProject/Fido.git
```

### With Simulator

Fido's simulator requires [SFML](http://www.sfml-dev.org/learn.php). If you would like the simulator, install SFML.

If you are on a **Unix** machine, use the install script.
```
$ cd Fido/ && bash install.sh
```

If you are on **Windows**, use
```
$ cd Fido/ && install.bat
```

### Without Simulator

If you do not want the simulator use the install script with the `-noSFML` argument
```
$ cd Fido/ && bash install.sh -noSFML
```

Done! To use Fido, just link the `fido-lib.a` file and include any headers from the `Software` directory.

## Example Code

An example of creating and training a neural network to perform linear regression.

```cpp
#include "NeuralNet.hpp"
#include "Backpropagation.hpp"
#include <iostream>

int main() {
  // Creates a neural network with
  // 1 input, 1 output, 2 hidden layers, 4 neurons per hidden layer,
  // a sigmoid activation function for hidden layers, and a linear activation function on the final layer.
  net::NeuralNet neuralNetwork = net::NeuralNet(1, 1, 2, 4, "sigmoid");
  neuralNetwork.setOutputActivationFunction("simpleLinear");

  std::vector< std::vector<double> > input = { {1}, {2}, {5}, {6} };
  std::vector< std::vector<double> > correctOutput = { {2}, {4}, {10}, {12} };

  // Create backpropagation object with
  // a learning rate of 10%, a momentum term of 0.001, an acceptable error level of 0.1%,
  // and a maximum number of training iterations of 10000
  net::Backpropagation backprop = net::Backpropagation(0.1, 0.001, 0.001, 10000);
  backprop.setDerivedOutputActivationFunction("simpleLinear");
  backprop.trainOnData(&neuralNetwork, input, correctOutput);

  // Cycle through inputs and print the outputs
  for (std::vector<double> current: input) {
      std::cout << "Input: " << current[0] << "\tNeural network's answer:" << neuralNetwork.getOutput(current)[0] << std::endl;
  }
}
```

## Contributing

Send us a pull request. If you are looking for things to do, check out the repo's open issues. We will be happy to add you as a contributor and credit you in the README.

If you find a bug or have any trouble with the library, please open an issue. We are happy to help you out.

### Authors

Fido was created by [Michael Truell](https://github.com/truell20) and [Joshua Gruenstein](https://github.com/joshuagruenstein).

### Contributors

Thanks to [Henry Wildermuth](https://github.com/FlyingGraysons) for his contributions.
