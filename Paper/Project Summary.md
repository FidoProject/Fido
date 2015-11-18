###What is the RATIONALE for your project? Please include a brief synopsis of the background research that supports your research problem and explain why this research is important scientifically and, if applicable, explain any potential societal impact of your research. Please include citations in your project rationale. (750 words max)

In practice most robot control systems are procedural programmed systems; conditional logic is executed linearly to emulate a desired behavior.  Structures such as functions, if statements, and classes are used to implement the ability for a robot to perform a desired task.   While this method may be sufficient for performing repetitive logic-based tasks such as line-following, it is clearly not scalable to higher level intelligent functionality.   Additionally, this system lacks the ability to interact with its operator, and will always perform the same task rather than  dynamically adjusting its task based on further training.  Finally, such traditional control systems must be completely rewritten for different robots and their respective inputs and outputs.

The rationale behind our project was to fix these issues and create a trainable robot control system that is both scalable and portable (able to run on any robot/platform).  We named the control system Fido for the name’s connotations to training an intelligent organism.  Fido is based on artificial neural networks, chosen for their high scalability and trainability.  We modified an algorithm called "Q-Learning" to be suitable for Fido, which performs low-level learning functionality.

<!-- 
Fido has been successfully applied to numerous real-world functions, such as being an autonomous player in the video game Halite and acting as "pet robot" able to be trained like a dog.
--> 

###State your HYPOTHESIS(ES) / RESEARCH QUESTION(S) / ENGINEERING GOAL(S). Describe how your research question(s), hypothesis(es) and/or goal(s) build on the research described in your project rationale. (250 words max) 

###Describe in detail your research methods and conclusions. (500 words max)
- Procedures/Data Collection: Detail experimental design, including all procedures used for data collection. Be sure to describe in detail only those methods and procedures you (and your teammates) conducted, and not those of your mentor, teacher, or from any other researcher.
- Data Analysis: Describe the procedures to be used to analyze your data and answer your research question(s). Be sure to address all questions in Part B that are relevant to your research project.
