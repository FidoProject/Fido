#include "SoundTask.h"

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

	*stateSize = 1, *actionDimensions = 2, *numberOfActions = 4, *neuronsPerLayer = 10, *numberOfLayers = 3;
	*beginningExplorationConstant = 0.2, *explorationConstantDevaluation = 1;
	*minAction = { 0, 0 }, *maxAction = { 1, 1 };
	*baseOfDimensions = 6;
}

std::vector<double> SoundTask::getState() {
	return {hardware->getMicrophone()};
}

double SoundTask::performAction(const std::vector<double> &action) {
	hardware->setMotors(action[0]*100, action[1]*100);

	return connection->getReward();
}

bool SoundTask::isTaskDone() {
	return false;
}

void SoundTask::reset() {

}
