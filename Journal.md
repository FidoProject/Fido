# Fido Research Journal

## Progress made over the summer
### Software - Michael Truell
We began by familiarizing ourselves with the scientific literature on neural networks and their uses. I began with a fabulous tutorial on using neural networks paired with a genetic algorithm to solve tasks (http://www.ai-junkie.com/ann/evolved/nnt1.html). Then general implementations of a neural network and a genetic algorithm were written. To test these two pieces of software, neural networks were taught to play tic tac toe by playing against one another and by being subject to our genetic algorithm. Once this was done, I familiarized myself with backpropagation, an algorithm widely used to train neural networks given a set of training data, by reading a number of introductory papers on the subject. After this, an implementation of backpropagation was written and added to our code base. It was tested by teaching a neural network to perform some simple linear regression. To further test the power of our software, we used our genetic algorithm implementation to teach neural networks to play Halite, a game for AIs made by our friend and classmate Benjamin Spector (10th grade), and used our backpropagation implementation to teach neural networks how to perform linear and nonlinear regression.

We then turned our sights on the topic of our project, reinforcement learning. We implemented the popular algorithm Q-learn, for teaching function approximators (a feedforward neural network is just a function approximator) discrete actions. We added in some non-random search heuristics, so that our NNs could learn in large state and action spaces. We tested our implementations on simple tasks, like turning on an LED when prompted with the proper stimuli (like a flash of light), using a simulator that we built. However, though our q-learning implementation can handle continuous state spaces, our neural networks will have to carry out continuous actions (like the setting of motor values), and so it was necessary to extend our Q Learning implementation into one that could handle continuous action spaces. We turned to the use of a wire-fitted least squares interpolator couple with a neural network as described in Gasket et al. (http://users.cecs.anu.edu.au/~rsl/rsl_papers/99ai.kambara.pdf). We are currently testing and modifying this algorithm so that it converges on the correct method for completing a task with less human feedback

### Hardware - Joshua Gruenstein

Our original idea for Fido's body was a "black box," an object seemingly lacking in potential that could had hidden abilities the neural networks could learn to utilize.  However, due to the practical issues with the locomotion of a cube without wheels we decided to pursue our next best option, a sphere.  We decided to have a differential drive robot inside of a sphere, an easy kinematic model to learn: left motor pushes right, right motor pushes left, and combining these vectors pushes straight.  For inputs we chose easy manipulatable sensors for training purposes: light and a microphone were obvious initial choices.  We also decided to add an infrared phototransistor as another form of one dimensional gradient stimulus (the intensity of the IR) that could be used during training.  Battery level also seemed like an easy choice as it mimics nature in an interesting way.  We also decided to add an IMU, or inertial measurement unit.  This is an all in one package which generally contains three axis of accelerometers, gyroscopes, and magnetometers.  In addition to calculating our orientation in 3D space from these sensors, we can also use the raw readings for various purposes: as a high acceleration (after compensating for gravity) is generally indicative of a high force according to F=ma, a high acceleration can be used as a negative reinforcement.  The same thinking was used with gyroscopic readings and dizziness.  We decided to use the magnetometer similarly to IR as an easily controlled stimulus, but now in three dimensions.

Multiple computing cores were considered for the project, but we decided to go with an ARM Cortex M4 micro-controller devboard called Teensy 3.1.  The Teensy is open source, powerful, and can be programmed in C++ without having to muddle through Linux or any other operating system; with a micro-controller you write your own firmware.  A schematic was made in Eagle CAD and a PCB routed.  The boards were fabricated by Smart Prototyping in Shenzhen, and are set to arrive September 9th.  When they arrive they'll need to be assembled with parts ordered from Digikey and Mouser, two bulk electronics suppliers.  It'll be a lot of soldering.  So much fun.

##Post Summer Research

### September 7, 2015
#### Michael Truell
A WireFitRobot class was created that connected our of Wire Fit Q Learn implementation to our simulator. This was taught a number of simple tasks, such as to drive as fast as possible and to drive straight. Learning to drive as fast as possible was defined as choosing a speed above 95% for five iterations of feedback in a row and learning to drive straight was classified as having a difference of motor values of less than 0.05 for five iterations of feedback in a row. However, the number of inputs required by a human subject before the robot learned successfully is quite high. For a WireFitQLearn object with 3 wires, it was on the order of 50 to learn drive straight and around 15 to drive fast. In contrast, for our discrete Q-Learn implementation it took less than 10 feedback iterations for it to learn to only turn on an LED when triggered by the proper stimulus.

This can surely be fixed with modifications to the present Wire Fit Q-learn implementation. 

### September 10, 2015
#### Michael Truell
Right now, a reward is tagged to the last action taken, which is used to update the Q-value (or long term reward) of the action. Stochastic gradient descent is used to modify the control points outputted by the neural network, so that the control points produce an interpolator function that accurately outputs the Q-value of the last action.

I have a few ideas as to how to modify WireFitQLearn for the better:

- The loss function for the gradient descent of control points should look to minimize error in matching reward-action pairs from the past. However, reward-action pairs should be devalued based on how old they are by giving them a greater acceptable error value (the distance the output of the function produced from gradient descent is from the correct reward value) as they age. This is necessary so that the WireFitQLearn object can "forget."

- Past reward-action value pairs can be stored, and the control points can be fitted to all historical data points individually every reward iteration. The historical data points will be in order starting from oldest to newest.

- Past reward-action value pairs can be stored, and the control points can be fitted to all historical data point individually every reward iteration. The historical data points will be shuffled each reward iteration.

- Past reward-action value pairs can be stored, and the control points can be fitted on historical data points a set number of times. Data points will be chosen using a boltzman soft-max distribution that favors newer data points.

### September 11, 2015
#### Joshua Gruenstein

After meeting with Dr. Weitz D period today, we've decided to apply to the Siemens competition as well.   This modifies our existing plan, as we now have to complete a submittable project in 11 days (the deadline for paper submission is September 22, and you cannot resubmit your paper with more research).  Firstly, we're switching to a fully simulator based paper and upgrading our simulator to be more full featured.  Rather than simply displaying motor value vectors, we will simulate a dynamic model of the robot in 2D space using differential drive kinematics.   We also need to finish adapting our wirefit q-learning method to use less data points.  Below is a todo list for the next 11 days:

#####Technical
- Make model learn with less datapoints
	- Modify wirefit q-learn and check effectiveness
	- Switch to adaptive critic if need be
- ~~Simulator kinematics~~

#####Paper
- Siemens paperwork
- Abstract
- Introduction
- Software section
	- Writing about neural networks
	- Backpropogation
	- Q-learn implementation
	- Modifications to q-learn
- Simulator
 	- Layout
 	- Kinematics 
 	- Inputs
- Test data
	- How many iterations for learning tasks
- Conclusion
	- Summary
	- Talk about future (hardware/pcb)  
- Figures
	- ~~MLP (feedforward neural network)~~
	- ~~Activation functions~~
	- Back propagation of error
	- Diagram of interpolator with NN
	- Some representation of modifications to Q-learn
	- Test results graph (bar graph + table)
	- Psuedocode/algorithms
		- Q-learn
		- Wirefit q-learn
		- Modified wire-fit q-learn

### September 13, 2015
#### Joshua Gruenstein

On Friday I began the implementation of the kinematic simulation of a differential drive robot.  As reference I used UC Berkeley class notes compiled from "Dudek and Jenkin, Computational Principles of Mobile
Robotics."  I later had to use the book itself and found a copy on Google books.  The implementation was fairly simple,  and can simply be described as such: determine the circle around which's circumference the robot will turn, find the center of the robot on said circumference, rotate around the origin by the curve at that point around circle, make the linear transformation, than transform back to standard space.  A robot class was created with a go(motorLeft,motorRight) method to input motor values in a traditional fashion.  While I initially encountered issues regarding the origin of the robot shape, after compensating for this I was able to get the kinematic model to respond beautifully.  I hooked it up to some keys on my keyboard and could drive it around like a slightly less exciting racing game.

In addition to the sensors already implemented in our simulator, we also wanted the ability to train a robot to follow an object, such as an emitter of electromagnetic radiation on some part of the spectrum.  However doing a "hot and cold" type adjustment on sliders seemed impractical, and instead the ability to graphically add a radio beacon to the simulation environment was added instead.  Right clicking added/moved the beacon, giving off some sort of field.  As this is a simulation the type of radiation is purposefully vague, but readings from the robot are separated into an X component and a Y component and follow an inverse square law.  The hope is to use this tool to train Fido to follow this beacon by associating larger sensor readings with positive reinforcement.

I then realized that as the robot is accelerating and turning, the simulated movement should affect accelerometer and gyroscopic sensor readings.  I attempted to implement acceleration to the kinematic model for numerous hours (it's quite complex, considering each motor accelerates independently and differently according to load, position, etc).  However after realizing that we would probably compensate for this ceteris paribus in a real robot, I decided to abandon this venture.

As I've completed the technical side of my part of the work for the fast approaching Siemens deadline, I'm currently working on writing about and making figures for our Neural Network implementation in our paper.

#### Michael Truell

These past 2 days I have set about implementing the four modifications to our Wire fitted Q-learning implementation as specified in my last journal entry. I began by trying to implement my first idea of fitting the control points to many data points simultaneously. However, this modification would become useless on any task with a large state space, such as following the origin of an electromagnetic emitter, and as such, is of no use to us.

I then implemented my other three proposed modifications and tested them by recording the number of reward iterations it took for Fido to learn to drive straight when each was in place. Learning to drive straight was defined as having a difference in 5 between Fido's motor values for 5 reward iterations in a row. However, none of the three seemed to improve the performance of our implementation at learning to drive straight. Convergence still took 160 reward iterations on average.

The problem lay with the task that I chose for Fido to learn. The graph that represents that task [ ex. ([right motor, left motor], reward values) ] is linear. Our implementation was using a least squares interpolator, which cannot form a straight line. As such, I though that we would need to have a way to pick and chose the interpolator that we would want WireFitQLearn (our continuous state-action space implementation of Q-Learning) to use for each task. I wrote a base class for all interpolators called Interpolator that defined virtual functions that needed to be implemented by each interpolator. I then detached the least squares interpolator functionality from WireFitQLearn and made it into its own class called WireFitInterpolator.

My next tasks are:

- Implementing a linear interpolator and measuring its performance at learning to drive straight
- Testing my modifications to WireFitQLearn while using a suitable interpolator

###September 23, 2015 at 12:01
#### Joshua Gruenstein

We've submitted to Siemens.  As we were rushing we didn't have much time to document the massive amount of work we were doing in the journal, but the code documentation has been continued.  Over the next couple of days we will back-document what happened in these hectic last weeks.
