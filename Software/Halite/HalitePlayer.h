#ifndef HALITEPLAYER_H
#define HALITEPLAYER_H

#include <iostream>
#include <time.h>
#include <set>

#include "../NeuralNet.h"
#include "Networking.h"
#include "hlt.h"

class HalitePlayer {
public:
	HalitePlayer();
	HalitePlayer(net::NeuralNet *network_);
	void connect();
	int playGames();
	hlt::Move getMove(hlt::Location sentientLocation);

	net::NeuralNet *network;
private:
	unsigned char my_tag;
	unsigned char age_of_sentient;
	sf::TcpSocket * connection;
	hlt::Map present_map;
	std::set<hlt::Move> moves;
};

#endif