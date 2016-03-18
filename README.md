# Fido

An open-source, highly modular C++ machine learning library for embedded electronics and robotics. Fido includes implementations of neural networks, reinforcement learning, and genetic algorithms. Fido is especially suited for robotic contexts, as it is written in C++ with minimal use of the standard library, comes packaged with a robotic simulator, and provides and easy interface in which to write robotic drivers.

## Getting Started

Clone the github repository.
```
$ git clone https://github.com/FidoProject/Fido.git
```

Move to the Software directory and build the library using cmake.
```
$ cd Fido/Software && make
```

Now you may use Fido in your C++ projects, by including any of the header files located in the Software directory.

## Example Code

### Linear Regression 

An example of creating and training a neural network to perform linear regression.

```cpp
#include "Fido/Software/NeuralNet.h"
#include "Fido/Software/Backpropagation.h"

int main() {
  // Creates a neural network with 
  // 1 input, 1 output, 2 hidden layers, 4 neurons per hidden layer, and a sigmoid activation function.
  net::NeuralNet neuralNetwork = NeuralNet(2, 1, 2, 4, "sigmoid");
  std::vector< std::vector<double> > input = { {1}, {2}, {5}, {6} };
  std::vector< std::vector<double> > correctOutput = { {2}, {4}, {10}, {12} };
  
  // Create backpropagation object with 
  // a learning rate of 10%, a momentum term of 0.001, an acceptable error level of 5%, 
  // and a maximum number of training iterations of 10000
  net::Backpropagation backprop = Backpropagation(0.1, 0.001, 0.05, 10000);
  backprop.trainOnData(&network, input, correctOutput);
}
```

### Stay Still

An example of teaching a Robot in the simulator to stay still when light is shone on it. Uses reinforcement learning, specifically Q-Learning.

First, implement a task for the robot.

```cpp
#include "Fido/Software/FidoSim/Simlink.h"

StayStillTask::StayStillTask(Simlink *simulator_) { 
  simulator = simulator_; 
};

void StayStillTask::getRobotParameters(int *stateSize,
						int *actionDimensions,
						int *numberOfActions,
						int *neuronsPerLayer,
						int *numberOfLayers,
						double *beginningExplorationConstant,
						double *explorationConstantDevaluation,
						std::vector<double> *minAction,
						std::vector<double> *maxAction,
						double *baseOfDimensions) {
  *stateSize = 1;
  *numberOfActions = 3, *neuronsPerLayer = 10, *numberOfLayers = 3;
  *beginningExplorationConstant = 0.15, *explorationConstantDevaluation = 1;
  *actionDimensions = 1;
  *minAction = { 0 }, *maxAction = { 1 };
  *baseOfDimensions = 2;
}

std::vector<double> StayStillTask::getState() { 
  return { simulator.getVis() / 100.0 }; // Light intensity is robot's input
}

double StayStillTask::performAction(const std::vector<double> &action) {
  double motorVal = action[0], lightVal = getState()[0];
  simulator->setMotors(motorVal, motorVal, 1, 1); // Move robot
  if((motorVal == 0 && lightVal > 0.5) || (motorVal == 1 && lightVal < 0.1)) return 1; // Did something right
  else return -1; // Did something wrong
}

bool StayStillTask::isTaskDone() { 
  return false; // Never stop training (until trial is stopped)
}

void StayStillTask::reset() {}
```

Then run a robot that trains on that task.

```cpp
#include "Fido/Software/Robot.h"
#include "Fido/Software/FidoSim/Simlink.h"
int main() {
  Robot robot = Robot(new StayStillTask(new Simlink()));
  // Run 10 trials. Stop each trial after 100 pieces of reward have been given to the roobt.
  robot.run(10, 100);
}
```


## Contributing

Send us a pull request. If you are looking for things to do, check out the repo's open issues. We will be happy to add you as a contributor and credit you in the README.

If you find a bug or have any trouble with the library, please open an issue. We are happy to help you out.

## Authors

Fido was created by [Michael Truell](https://github.com/truell20) and [Joshua Gruenstein](https://github.com/joshuagruenstein).
