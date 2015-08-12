#ifndef HALITE_H
#define HALITE_H

#include <iostream>

#include "../NeuralNet.h"
#include "../GeneticAlgo.h"

class Halite {
public:
	static net::NeuralNet getBestPlayer(int numberOfIterations);
};

#endif