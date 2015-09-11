# Fido Research Journal

## Progress made over the summer
### Software - Michael Truell
We began by familiarizing ourselves with the scientific literature on neural networks and their uses. I began with a fabulous tutorial on using neural networks paired with a genetic algorithm to solve tasks (http://www.ai-junkie.com/ann/evolved/nnt1.html). Then general implementations of a neural network and a genetic algorithm were written. To test these two pieces of software, neural networks were taught to play tic tac toe by playing against one another and by being subject to our genetic algorithm. Once this was done, I familiarized myself with backpropagation, an algorith widely used to train neural networks given a set of training data, by reading a number of introductory papers on the subject. After this, an implentation of backpropagation was written and added to our code base. It was tested by teaching a neural network to perform some simple linear regression. To further test the power of our software, we used our genetic algorithm implemenatation to teach neural networks to play Halite, a game for AIs made by our friend and classmate Benjamin Spector (10th grade), and used our backpropagation implementation to teach neural networks how to perform linear and nonlinear regression.

We then turned our sights on the topic of our project, reinforcement learning. We implemented the popular algorithm Q-learn, for teaching function approximators (a feedforward neural network is just a function approximator) descrete actions. We added in some non-random search heuristics, so that our NNs could learn in large state and action spaces. We tested our implementations on simple tasks, like turning on an LED when prompted with the proper stimuli (like a flash of light), using a simulator that we built. However, though our q-learning implementation can handle continous state spaces, our neural networks will have to carry out continuous actions (like the setting of motor values), and so it was neccessary to extend our Q Learning implementation into one that could handle continuous action spaces. We turned to the use of a wire-fitted least squares interpolator couple with a neural network as described in Gasket et al. (http://users.cecs.anu.edu.au/~rsl/rsl_papers/99ai.kambara.pdf). We are currently testing and modifying this algorithm so that it converges on the correct method for completing a task with less human feedback

### Hardware - Joshua Gruenstein

Our original idea for Fido's body was a "black box," an object seemingly lacking in potential that could had hidden abilities the neural networks could learn to utilize.  However, due to the practical issues with the locomotion of a cube without wheels we decided to persue our next best option, a sphere.  We decided to have a differential drive robot inside of a sphere, an easy kinematic model to learn: left motor pushes right, right motor pushes left, and combining these vectors pushes straight.  For inputs we chose easy manipulatable sensors for training purposes: light and a microphone were obvious initial choices.  We also decided to add an infrared phototransistor as another form of one dimensional gradient stimulus (the intensity of the IR) that could be used during training.  Battery level also seemed like an easy choice as it mimicks nature in an interesting way.  We also decided to add an IMU, or inertial measurement unit.  This is an all in one package which generally contains three axis of accelerometers, gyroscopes, and magnetometers.  In addition to calculating our orientation in 3D space from these sensors, we can also use the raw readings for various purposes: as a high acceleration (after compensating for gravity) is generally indicative of a high force according to F=ma, a high acceleration can be used as a negative reinforcement.  The same thinking was used with gyroscopic readings and diziness.  We decided to use the magnetometer similiarly to IR as an easily controlled stimulus, but now in three dimensions.

Multiple computing cores were considered for the project, but we decided to go with an ARM Cortex M4 microcontroller devboard called Teensy 3.1.  The Teensy is open source, powerful, and can be programmed in C++ without having to muddle through Linux or any other operating system; with a microcontroller you write your own firmware.  A schematic was made in Eagle CAD and a PCB routed.  The boards were fabricated by Smart Prototyping in Shenzhen, and are set to arrive September 9th.  When they arrive they'll need to be assembled with parts ordered from Digikey and Mouser, two bulk electronics suppliers.  It'll be a lot of soldering.  So much fun.

##Post Summer Research

### September 7, 2015
#### Michael Truell
A WireFitRobot class was created that connected our of Wire Fit Q Learn implementation to our simulator. This was taught a number of simple tasks, such as to drive as fast as possible and to drive straight. Learning to drive as fast as possible was defined as choosing a speed above 95% for five iterations of feedback in a row and learning to drive straight was classified as having a difference of motor values of less than 0.05 for five iterations of feedack in a row. However, the number of inputs required by a human subject before the robot learned succesfully is quite high. For a WireFitQLearn object with 3 wires, it was on the order of 50 to learn drive straight and around 15 to drive fast. In contrast, for our discrete Q-Learn implementation it took less than 10 feedback iterations for it to learn to only turn on an LED when triggered by the proper stimulus.

This can surely be fixed with modifications to the present Wire Fit Q-learn implementation. 

### September 9, 2015
#### Michael Truell
Right now, a reward is tagged to the last action taken, which is used to update the Q-value (or long term reward) of the action. Stocastic gradient descent is used to modify the control points outputed by the neural network, so that the control points produce an interpolator function that accurately outputs the Q-value of the last action.

I have a few ideas as to how to modify WireFitQLearn for the better:

- The loss function for the gradient descent of control points should look to minimize error in matching reward-action pairs from the past. However, reward-action pairs should be devalued based on how old they are by giving them a greater acceptable error value (the distance the output of the function produced from gradient descent is from the correct reward value) as they age. This is neccessary so that the WireFitQLearn object can "forget."

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
- Simulator kinematics

#####Paper
- Introduction
- Software section
	- Writing about neural netowkrs
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
	- Back propogation of error
	- Diagram of interpolator with NN
	- Some representation of modifications to Q-learn
	- Test results graph (bar graph + table)
	- Psuedocode/algorithms
		- Q-learn
		- Wirefit q-learn
		- Modified wirefit q-learn
 