# Fido

## Mission Impossible: Submit by Tuesday

### Tasks
- Evaluate current state of Fido
  - ~~Statically type hyperparameters (nn topology, number of actions, boltzman devaluation and starting constant)~~
  - ~~Evaluate performance on driving toward an emmiter and setting an LED to the right color when given the proper stimulus~~
  - Determine if there is a need to dynamically change hyperparameters
- ~~Separate tasks from WireFitRobot class for code reuse~~
  - ~~Create virtual class and pass in objects of this class to WireFitRobot~~
  - ~~Create Tasks for common tasks~~
    - ~~Float to emitter~~
    - ~~Drive to emitter~~
    - ~~LED to right color when prompted~~
    - Drive in a shape
    - Line following using the light sensor
- Implement dynamic changing of number of control wires
  - Calculate bias (error at predicting reward) and variance (deviation from the mean)
  - Use brute force descent (change the number of control wires see if variance changes favorably) to determine the proper number of control wires
  - When lowering or raising the number of control wires, take the new set of control wires (add or subtract one fromt the old set) and use gradient descent to minimize error in modeling the interpolator function with the past control points
    - Determine derivative of cost function (its just the summation of the derivatives of the difference between each functions ranges squared)
    - Perform gradient descent
    - Acceptable error level will be set low (0.001) and maxIterations will be low (1000)
- **Paperwork**
  - **Excecutive summary**
  - Schiller's forms
  - Weitz's forms
- Paper
  - Learning Impl
  - Results
  - Discussion
  - Conclusion
  - Introduction
  - Abstract

## Future TODO
- Implement more tasks
  - Have to instances of Fido reproduce
  - Place two instances of Fido in a simulation with limited food; Give them the ability to kill
  - Use genetic algorithms to actually have instances of Fido reproduce and evolve

### Papers used for References
 * Q Learning for Continous State Action Spaces - http://users.cecs.anu.edu.au/~rsl/rsl_papers/99ai.kambara.pdf
 * First Description of Backpropagation - Beyond Regression: New Tools for Prediction and Analysis in the Behavioral Sciences by Paul Werbos
 * Backpropagation Tutorial - https://www4.rgu.ac.uk/files/chapter3%20-%20bp.pdf
 * Backpropagation Tutorial - http://www.cse.unsw.edu.au/~cs9417ml/MLP2/
 * Q Learning Implementation - http://web.cs.swarthmore.edu/~meeden/cs81/s12/papers/MarkStevePaper.pdf
 * Boltzmann - http://computing.dcu.ie/~humphrys/PhD/ch2.html
