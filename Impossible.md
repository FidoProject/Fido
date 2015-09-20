## Mission Impossible: Submit by Tuesday

### Tasks
- Evaluate current state of Fido
  - Statically type hyperparameters (nn topology, number of actions, boltzman devaluation and starting constant)
  - Evaluate performance on driving toward an emmiter and setting an LED to the write color when given the proper stimulus
  - Determine if there is a need to dynamically change hyperparameters
- Separate tasks from WireFitRobot class for code reuse
  - Create virtual class and pass in objects of this class to WireFitRobot
- Implement dynamic changing of hyperparameters
  - Calculate bias (error at predicting reward) and variance (deviation from the mean)
  - Use brute force descent (change the number of actions see if variance changes favorably) to determine the proper number of actions
- **Paperwork**
  - **Excecutive summary**
