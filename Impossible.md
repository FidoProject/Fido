## Mission Impossible: Submit by Tuesday

### Tasks
- Evaluate current state of Fido
  - Statically type hyperparameters (nn topology, number of actions, boltzman devaluation and starting constant)
  - Evaluate performance on driving toward an emmiter and setting an LED to the right color when given the proper stimulus
  - Determine if there is a need to dynamically change hyperparameters
- Separate tasks from WireFitRobot class for code reuse
  - Create virtual class and pass in objects of this class to WireFitRobot
- Implement dynamic changing of number of control wires
  - Calculate bias (error at predicting reward) and variance (deviation from the mean)
  - Use brute force descent (change the number of control wires see if variance changes favorably) to determine the proper number of control wires
  - When lowering or raising the number of control wires, take the new set of control wires (add or subtract one fromt the old set) and use gradient descent to minimize error in modeling the interpolator function with the past control points
    - Determine derivative of cost function (its just the summation of the derivatives of the difference between each functions ranges squared)
    - Perform gradient descent
    - Acceptable error level will be set low (0.001) and maxIterations will be low (1000)
- **Paperwork**
  - **Excecutive summary**
