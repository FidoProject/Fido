# Fido

Researched and built by [Michael Truell](https://github.com/truell20 "Michael Truell") and [Josh Gruenstein](https://github.com/joshuagruenstein "Josh Gruenstein").

Fido is a universal robotic control system using reinforcement learning with limited feedback for NYCSEF 2016 and the Science Research class at Horace Mann School. Fido was researched and built in C++ by Josh Gruenstein and Michael Truell.

## Project Summary

Robotic intelligence can be simplified to a black box, with inputs such as sensor data and outputs such as motor control.  Most robotic software today operates as an "expert system," using preprogrammed logic to execute a set routine.  These implementations are sufficient for the specific purpose and platform that they are designed for but lack the ability to perform other tasks or work on other robots.

The purpose of this project was to develop a universal robot control system that does not require preprogrammed logic to accomplish specific tasks, can be adapted to any robot hardware, and can be trained in a short amount of time using positive and negative reinforcement.  The control system we developed (nicknamed Fido) is lightweight and resource-efficient, making it a practical solution for mobile robots.  This was achieved by employing an artificial neural network and a novel learning algorithm.  The algorithm takes a well-tested learning algorithm called Q-learning and modifies it to be better suited for robotic tasks.

Fido was tested on a computer simulated robot with a motor control system, a large sensor array, and some additional outputs.  The system performed well doing a variety of tasks, such as learning to drive to a radio beacon and following a line.  Additionally, results showed that very little sensory data was necessary and training took very little time.  Future plans for continuing research include testing Fido using actual robotic hardware and additional improvements to Fido's speed and abilities.

## Abstract

A robot control system was developed that could be taught tasks through reinforcement learning.  The system, nicknamed Fido, was designed to be universal regardless of the specific hardware inputs and outputs and does not need to be modified for the task at hand. In addition, Fido was built to learn with limited feedback, allowing humans to train Fido in a reasonable time frame. This was achieved through the training of artificial neural networks with a wire-fitted moving least squares interpolator following the Q-learning reinforcement algorithm and an action selection policy that utilizes a Boltzmann distribution of probability. A kinematically accurate robot was simulated with a differential drive system, a sensor array, and other outputs.  The robot was successfully trained to do an array of tasks with limited feedback, such as line following, driving to a set location, and blinking a multicolor light-emitting diode.

## Summary of Results

### Experiments

To test Fido's effectiveness at learning with limited feedback, Fido was trained on a number of different tasks through our simulator using reward values delegated by our software. Data was collected regarding Fido's latency and number of reward iterations needed for convergence.

Fido's first and simplest task, dubbed "Flash," was to set the brightness value of an LED to a value proportional to the amount of light that Fido sensed. Each reward iteration, Fido's neural network was given the intensity of visible light that Fido detected and was asked for the brightness value of Fido's LED.

"Float," Fido's second task, challenged our learning implementation to direct a robot to point. Each time it was told to select an action, Fido specified the robot's vertical and horizontal velocity between +10 and -10 pixels. This emulates a holonomic drive systems, where motor outputs directly correlate to movement on the $x$ and $y$ axes. At the start of each trial, Fido and the point were placed randomly on a boundless plane within 768 pixels of one another.

Fido's next task, nicknamed "Drive," required that it direct a robot to point by controlling the motors of a differential drive system. At the start of each trial, Fido and the point were placed randomly on a boundless plane within 768 pixels of one another.

Fido's last challenge, called "Follow," was to perform the classic robotic task of line following by controlling the motors of the simulator's differential drive system. At the start of each trial, Fido and a line with a random rotation were placed arbitrarily on a boundless plane within 768 pixels of one another. Fido completed each trial when it had stayed within 60 pixels of the line for 10 consecutive actions (5 reward iterations).

### Findings

#### Simulator Findings

|Task     | Learning Iterations     | Action Selection (ms)     | Training Time (ms)      |
--- | --- | --- | --- | ---
|Flash       | 6                   | 0.                  | 6               |
|Float       | 14                  | 1                  | 6               |
|Drive       | 17                  | 1                  | 11              |
|Line Follow | 21                  | 2                  | 10.             |

Each task was run 500 times to gather the data show in the above table.  The reward iterations values shown in the above data table were the medians of the data collected. The median is shown instead of the mean to discount a few large outliers that were present in data. The time data shown above is the mean of the data collected.

The data collected demonstrates the prowess of the Fido control system.   Fido was able to master both a holonomic and differential-drive motor control system (the "float" and "drive" tasks), proving its hardware agnostic capabilities.  All tasks showed low numbers of reward iterations and low latency, allowing Fido to learn quickly and efficiently. Even the task that was most difficult to Fido, "Follow," took a median of 21 reward iterations, well within the patience of a human.

#### Hardware Findings
|Task     | Learning Iterations     | Action Selection (ms)     | Training Time (ms)      |
--- | --- | --- | --- | ---
|Stay Still       | 3                   | 1.                  | 43.5               |
|Drive to Point       | 18                  | 4                 | 65               |
