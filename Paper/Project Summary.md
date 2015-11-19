# Fido NYCSEF Project Summary
### Joshua Gruenstein and Michael Truell


-----------------------------------

###What is the RATIONALE for your project? Please include a brief synopsis of the background research that supports your research problem and explain why this research is important scientifically and, if applicable, explain any potential societal impact of your research. Please include citations in your project rationale. (750 words max)

In practice most robot control systems are procedural programmed systems; conditional logic is executed linearly to emulate a desired behavior.  Structures such as functions, if statements, and classes are used to implement a desired task.   While this method may be sufficient for performing repetitive logic-based tasks such as line-following, it is clearly not scalable to higher level intelligent functionality.   Additionally, this system lacks the ability to interact with its operator, and will always perform the same task rather than  dynamically adjusting its task based on further training.  Finally, such traditional control systems must be completely rewritten for different robots and their respective inputs and outputs. The rationale behind our project was to fix these issues and create a trainable robot control system that is both scalable and portable (able to run on any robot/platform).  

Fido is important both scientifically and societally.  Scientifically, our novel learning algorithm vastly reduces the amount of time that it takes to train a model; Fido can be trained complex tasks in minimal time.  Societally, Fido lowers robotics' barrier to entry of complex programming, as the control system can be uploaded to any robot hardware.  This allows tasks to be trained rather than programmed (which proves challenging for complex operations).

###State your HYPOTHESIS(ES) / RESEARCH QUESTION(S) / ENGINEERING GOAL(S). Describe how your research question(s), hypothesis(es) and/or goal(s) build on the research described in your project rationale. (250 words max) 

The goal of this project was to create a scalable, trainable, and portable robot control system.  To this respect the system could not have a "dynamic model" of the operating robot's hardware to provide physical guidelines for operation.  Additionally, the system could not be highly resource intensive or rely on external libraries, as this would prohibit operation on smaller mobile robots.  Positive and negative reinforcement could be used as a training mechanism, an analogue to real-world learning.  However the system must be able to operate with limited feedback; the robot must learn quickly (using as little enforcement as possible).


###Describe in detail your research methods and conclusions. (500 words max)
- Procedures/Data Collection: Detail experimental design, including all procedures used for data collection. Be sure to describe in detail only those methods and procedures you (and your teammates) conducted, and not those of your mentor, teacher, or from any other researcher.
- Data Analysis: Describe the procedures to be used to analyze your data and answer your research question(s). Be sure to address all questions in Part B that are relevant to your research project.

We named our control system Fido for the name’s connotations to training an intelligent organism, and implemented it in the C++ programming language.  Fido was based on artificial neural networks, chosen for their high scalability and trainability.  Fido used a modified version of the algorithm known as "Q-Learning" to provide its base-level learning functionality.  Care was taken to optimize Fido's codebase, ensuring that the system was both fast and light-weight.  

Next, a simulator was created to test Fido and gather performance metrics.  The simulator was asynchronous to best mimic robot hardware, with function calls to virtual inputs and outputs.   Data was collected for the number of learning iterations (enforcements) necessary for a variety of tasks, in addition to the time each learning iteration took.  It was found that Fido was highly competitive to other learning control systems, such as those utilizing discrete Q-Learning rather than Q-Learning in continuous action spaces using wire fitting.

Finally, a physical robot was constructed to both test interaction and training in person, in addition evaluating Fido's ability to learn robot hardware.  The robot used an Intel Edison SOC as its "brain" with a 3D printed chassis.  High microphone sensor readings, low light intensity and tilting were selected as negative reinforcements for the robot, making it effectively scared of the dark, averse to yelling, and easily nauseous.  An android application was created to aide interaction with Fido, allowing creation and selection of different neural networks.  Individual neural networks (nicknamed "tricks" to continue the Fido metaphor) could be used as a blank slate for learning, allowing a new task to be trained.   Tricks trained included making Fido stay still and making Fido balance on its two wheels.
