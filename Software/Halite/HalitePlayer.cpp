#include "HalitePlayer.h"

HalitePlayer::HalitePlayer() {
	network = new net::NeuralNet(1, 1, 1, 1, net::NeuralNet::sigmoid);
}

HalitePlayer::HalitePlayer(net::NeuralNet *network_) {
	network = network_;
}

void HalitePlayer::connect() {
	srand(time(NULL));
	connection = connectToGame();
	getInit(connection, my_tag, age_of_sentient, present_map);
	sendInitResponse(connection);
}

int HalitePlayer::playGames() {
	bool done = false;
	while(done == false)
	{
		moves.clear();
		getFrame(connection, present_map, done);
		if(done == true) break;
		bool noSentients = true;
		for(unsigned short a = 0; a < present_map.map_height; a++) {
			for(unsigned short b = 0; b < present_map.map_width; b++) {
				if(present_map.contents[a][b].owner == my_tag && present_map.contents[a][b].age == age_of_sentient) {
					noSentients = false;
					moves.insert(getMove({b, a}));
				}
			}
		}
		sendFrame(connection, moves);
		if(noSentients == true) std::cout << "No sen";
	}
	unsigned short wins = 0;
	getFinish(connection, wins);
	return wins;
}

hlt::Move HalitePlayer::getMove(hlt::Location sentientLocation) {
	std::vector< double > oneDInput;
	for(int y = sentientLocation.y - 1; y <= sentientLocation.y + 1; y++) {
		for(int x = sentientLocation.x - 1; x <= sentientLocation.x + 1; x++) {
			std::vector<double> inputsForLocation;
			unsigned short actualX, actualY;

			if(x < 0) actualX = x + present_map.map_width;
			else if(x > present_map.map_width - 1) actualX = x - present_map.map_width;
			else actualX = x;

			if(y < 0) actualY = y + present_map.map_height;
			else if(y > present_map.map_height - 1) actualY = y - present_map.map_height;
			else actualY = y;

			hlt::Site site = present_map.getSite({actualX, actualY});
			if(site.owner == my_tag) {
				if(site.age == age_of_sentient) inputsForLocation = { 1, 0 };
				else inputsForLocation = { 0, (double)site.age / age_of_sentient };
			} else if(site.owner == 0) {
				inputsForLocation = { 0, 0 };
			} else {
				if(site.age == age_of_sentient) inputsForLocation = { -1, 0 };
				else inputsForLocation = { 0, -1*((double)site.age / age_of_sentient) };
			}
			oneDInput.push_back(inputsForLocation[0]);
			oneDInput.push_back(inputsForLocation[1]);
		}
	}



	double bestMoveScore = -9999999;
	int bestMoveIndex = 0;
	unsigned char movesICanMake[] = {1, 3, 4, 5, 7};
	int middleIndex = 4 * 2;

	for(int a = 0; a < sizeof(movesICanMake) / sizeof(movesICanMake[0]); a++) {
		int currentIndex = movesICanMake[a] * 2;
		std::vector<double> holder = { oneDInput[currentIndex], oneDInput[currentIndex+1] };
		oneDInput[middleIndex] = 0; 
		oneDInput[middleIndex+1] = 0;
		oneDInput[currentIndex] = 1;
		oneDInput[currentIndex+1] = 0;

		double score = network->getOutput(oneDInput)[0];
		if(score > bestMoveScore) {
			bestMoveScore = score;
			bestMoveIndex = a;
		}
		oneDInput[middleIndex] = 1;
		oneDInput[middleIndex + 1] = 0;
		oneDInput[currentIndex] = holder[0];
		oneDInput[currentIndex + 1] = holder[1];
	}

	int moveNumber = -1;
	int move = movesICanMake[bestMoveIndex];
	if(move == 1) moveNumber = 1;
	else if(move == 3) moveNumber = 4;
	else if(move == 4) moveNumber = 0;
	else if(move == 5) moveNumber = 2;
	else if(move == 7) moveNumber = 3;

	return{ sentientLocation, moveNumber };
}