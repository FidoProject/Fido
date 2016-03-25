# Getting Started

Clone the github repository.
```
$ git clone https://github.com/FidoProject/Fido.git
```

Fido's robotic simulator is not necessary for the library's machine learning features. If you **would like** the simulator, install [SFML](http://www.sfmldev.org/learn.php) before proceeding.

If you **would not like** the simulator, run the command below with the `-noSFML` flag.		

If you are on a **Unix** machine, run
```
$ cd Fido/ && bash install.sh
```

If you are on **Windows**, run
```
$ cd Fido/ && install.bat
```

Done! To use Fido, just link the `fidolib.a` file and include any headers from the `Software` directory.

## Example Code

An example of creating and training a neural network to perform linear regression.

```cpp
#include "NeuralNet.h"
#include "Backpropagation.h"
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
	backprop.trainOnData(&neuralNetwork, input, correctOutput);

	// Cycle through inputs and print the outputs
	for (std::vector<double> current: input) {
		std::cout << "Input: " << current[0] << "\tNeural network's answer:" << neuralNetwork.getOutput(current)[0] << std::endl;
	}
}
```
