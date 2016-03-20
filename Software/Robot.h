#ifndef ROBOTN_H
#define ROBOTN_H

namespace rl {
	class Task;
	class Learner;
	
	/* Uses a learning model to progressively learn to to perform a task.
	 */
	class Robot {
	public:
		// Initializes a robot object
		Robot(Task *task_, Learner *learner_, double startingExplorationLevel_, double explorationDevaluationPerTimestep_);

		// Runs the robot for a specified number of time steps
		void run(int trials, int numberOfTimeSteps);

		void performAction();

	private:
		rl::Learner *learner;
		Task *task;
		double startingExplorationLevel, currentExplorationLevel, explorationDevaluationPerTimestep;

		void resetRobot();
	};
}

#endif
