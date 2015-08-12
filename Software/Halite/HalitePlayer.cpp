#include "HalitePlayer.h"

HalitePlayer::HalitePlayer(net::NeuralNet *network_) {
	srand(time(NULL));
	connection = connectToGame();
	getInit(connection, my_tag, age_of_sentient, present_map);
	sendInitResponse(connection);
}

void HalitePlayer::run() {
	bool done = false;
	while(done == false)
	{
		moves.clear();
		getFrame(connection, present_map, done);
		if(done == true) break;

		for(unsigned short a = 0; a < present_map.map_height; a++) {
			for(unsigned short b = 0; b < present_map.map_width; b++) {
				if(present_map.contents[a][b].owner == my_tag && present_map.contents[a][b].age == age_of_sentient) {
					moves.insert(getMove({b, a}));
				}
			}
		}
		sendFrame(connection, moves);
	}
	unsigned short wins = 0;
	getFinish(connection, wins);
	std::cout << "Number of wins: " << wins << "\n"; 
}


/// 
hlt::Move HalitePlayer::getMove(hlt::Location sentientLocation) {
	std::vector<double> oneDInput;
	for(int y = sentientLocation.y - 1; y < sentientLocation.y + 1; y++) {
		for(int x = sentientLocation.x - 1; x < sentientLocation.x + 1; x++) {
			hlt::Site site = present_map.getSite({x, y});
			if(site.owner == my_tag) {
				oneDInput.push_back((site.age / (age_of_sentient * 1.25)) + 0.2);
			} else if(site.owner == 0) {
				oneDInput.push_back(0);
			} else {
				oneDInput.push_back(-1 * ((site.age / (age_of_sentient * 1.25)) + 0.2));
			}
		}
	}

	double bestMoveScore = -9999999;
	int bestMoveIndex = 0;
	unsigned char movesICanMake[] = {1, 3, 4, 5, 7};
	for(int a = 0; a < sizeof(movesICanMake) / sizeof(movesICanMake[0]); a++) {
		int holder = oneDInput[movesICanMake[a]];
		oneDInput[4] = 0.2;
		oneDInput[movesICanMake[a]] = 1;
		double score = network->getOutput(oneDInput)[0];
		if(score > bestMoveScore) {
			bestMoveScore = score;
			bestMoveIndex = a;
		}
		oneDInput[4] = 1;
		oneDInput[movesICanMake[a]] = holder;
	}

	int moveNumber = -1;
	if(bestMoveIndex == 1) moveNumber = 1;
	else if(bestMoveIndex == 3) moveNumber = 4;
	else if(bestMoveIndex == 4) moveNumber = 0;
	else if(bestMoveIndex == 5) moveNumber = 2;
	else if(bestMoveIndex == 7) moveNumber = 3;

	return{ sentientLocation, moveNumber };
}