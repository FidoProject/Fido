# Fido

## Future Considerations
- Microcontroller-based hardware implementation
   - STM32L4?
- Implement more tasks
  - Have to instances of Fido reproduce
  - Place two instances of Fido in a simulation with limited food; Give them the ability to kill
  - Use genetic algorithms to actually have instances of Fido reproduce and evolve
- Implement dynamic changing of number of control wires
  - Calculate bias (error at predicting reward) and variance (deviation from the mean)
  - Use brute force descent (change the number of control wires see if variance changes favorably) to determine the proper number of control wires
  - When lowering or raising the number of control wires, take the new set of control wires (add or subtract one front the old set) and use gradient descent to minimize error in modeling the interpolator function with the past control points
    - Determine derivative of cost function (its just the summation of the derivatives of the difference between each functions ranges squared)
    - Perform gradient descent
    - Acceptable error level will be set low (0.001) and maxIterations will be low (1000)
