#ifndef HALITE_H
#define HALITE_H

#include <iostream>

#include "../NeuralNet.h"
#include "../GeneticAlgo.h"
#include "HalitePlayer.h"

class Halite {
public:
	static net::NeuralNet* getBestPlayer(int numberOfIterations);
	static std::vector<double> getPopulationFitnesses(std::vector<net::NeuralNet *> players);
	static int getScore(net::NeuralNet *network);

	static HalitePlayer player;
};

#endif