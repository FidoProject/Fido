# Fido

## Today TODO
- Demo videos
  - Finish Android App
  - Record training and executing tasks
      - Simulator: drive to point
      - Hardware: dance
  - Record simulator
  - Edit video
- Poster
  - Control system diagram figure
  - Fill out content
  - Print 

## Future TODO
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

### Papers used for References
 * Q Learning for Continous State Action Spaces - http://users.cecs.anu.edu.au/~rsl/rsl_papers/99ai.kambara.pdf
 * First Description of Backpropagation - Beyond Regression: New Tools for Prediction and Analysis in the Behavioral Sciences by Paul Werbos
 * Backpropagation Tutorial - https://www4.rgu.ac.uk/files/chapter3%20-%20bp.pdf
 * Backpropagation Tutorial - http://www.cse.unsw.edu.au/~cs9417ml/MLP2/
 * Q Learning Implementation - http://web.cs.swarthmore.edu/~meeden/cs81/s12/papers/MarkStevePaper.pdf
 * Boltzmann - http://computing.dcu.ie/~humphrys/PhD/ch2.html
