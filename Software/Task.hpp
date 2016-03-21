#ifndef TASK_H
#define TASK_H

#include <vector>

class Simlink;

namespace rl {
	// A virtual class that may be subclassed to implement a task to be carried out by WireFitRobot
	class Task {
	public:
		virtual std::vector<double> getState() = 0;
		virtual void performAction(const std::vector<double> &action) = 0;
		virtual double getRewardForLastStateAction();
		virtual bool isTaskDone() = 0;
		virtual void reset() = 0;
	};
}

#endif

