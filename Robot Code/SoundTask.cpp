#include "SoundTask.h"

#include <chrono>
#include <thread>

SoundTask::SoundTask(Hardware *hardware_, Connection *connection_) {
	hardware = hardware_;
	connection = connection_;
};

void SoundTask::getRobotParameters(int *stateSize,
						int *actionDimensions,
						int *numberOfActions,
						int *neuronsPerLayer,
						int *numberOfLayers,
						double *beginningExplorationConstant,
						double *explorationConstantDevaluation,
						std::vector<double> *minAction,
						std::vector<double> *maxAction,
						double *baseOfDimensions) {

	*stateSize = 1, *actionDimensions = 1, *numberOfActions = 4, *neuronsPerLayer = 10, *numberOfLayers = 3;
	*beginningExplorationConstant = 0.1, *explorationConstantDevaluation = 1;
	*minAction = { 0 }, *maxAction = { 1 };
	*baseOfDimensions = 3;

	isDone = false;
}

std::vector<double> SoundTask::getState() {
	double loudness = (hardware->getLoudness(100) - 0.4)/0.5;
	std::cout << "State: " << loudness << "\n";
	return {loudness};
}

void SoundTask::performAction(const std::vector<double> &action) {
	std::cout << "Action: " << action[0] << " " << action[0] << "\n";
	hardware->setMotors(action[0]*100, action[0]*100);

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	hardware->setMotors(0, 0);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

double SoundTask::getReward() {
	double reward = connection->getReward();
	std::cout << "Reward: " << reward << "\n";
	if(reward == -2) {
		isDone = true;
		return 0;
	}
	return reward;
}

bool SoundTask::isTaskDone() {
	return isDone;
}

void SoundTask::reset() {

}
