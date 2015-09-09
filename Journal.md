# Fido Research Journal

### Progress made over the summer
#### Software - Michael Truell
We began by familiarizing ourselves with the scientific literature on neural networks and their uses. I began with a fabulous tutorial on using neural networks paired with a genetic algorithm to solve tasks (http://www.ai-junkie.com/ann/evolved/nnt1.html). Then general implementations of a neural network and a genetic algorithm were written. To test these two pieces of software, neural networks were taught to play tic tac toe by playing against one another and by being subject to our genetic algorithm. Once this was done, I familiarized myself with backpropagation, an algorith widely used to train neural networks given a set of training data, by reading a number of introductory papers on the subject. After this, an implentation of backpropagation was written and added to our code base. It was tested by teaching a neural network to perform some simple linear regression.

#### Hardware

### September 7, 2015
#### Michael Truell
A WireFitRobot class was created that connected our of Wire Fit Q Learn implementation to our simulator. This was taught a number of simple tasks, such as to drive as fast as possible and to drive straight. Learning to drive as fast as possible was defined as choosing a speed above 95% for five iterations of feedback in a row and learning to drive straight was classified as having a difference of motor values of less than 0.05 for five iterations of feedack in a row. However, the number of inputs required by a human subject before the robot learned succesfully is quite high. For a WireFitQLearn object with 3 wires, it was on the order of 50 to learn drive straight and around 15 to drive fast.

This can surely be fixed with modifications to the present Wire Fit Q-learn implementation. Right now, a reward is tagged to an action, which is used to update the Q-value (or long term reward) of the action. Stocastic gradient descent is used to modify the control points outputed by the neural network, so that the control points produce an interpolator function that accurately outputs the reward.

The WireFitQLearn object can be modified for the better by instead changing the control points each reward iteration so that they have produce a function that not just accurately outputs the reward value given that iterations, but one that also will accurately output reward values values from past iterations when given their respective actions. Reward-action pairs should be devalued based on how old they are by giving them a greater acceptable error value (the distance the output of the function produced from gradient descent is from the correct reward value) as they age.
