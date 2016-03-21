#include <cstdio>
#include <iostream>
#include <algorithm>
#include "catch.hpp"
#include "../Software/NeuralNet.hpp"

inline bool operator==(const std::vector<double>& lhs, const std::vector<double>& rhs){
	if(lhs.size() != rhs.size()) {
		return false;
	}
	for(int a = 0; a < lhs.size(); a++) {
		if(fabs(lhs[a] - rhs[a]) > 0.00001) {
			return false;
		}
	}
	return true;
}

// Constants set for testing, could be anything
const int INPUTS(4);
const int OUTPUTS(2);
const int HIDDENS(3);
const int NUM_IN_HIDDENS(6);
const char * FILENAME = "dasgfgjhasdgkfhjkgfjaskfgh.dat";
const char * ACTIVATION_FUNCTION("sigmoid");
std::vector<double> input = {3, 5, 2, 5};

TEST_CASE( "Neural Network Contructor", "[network]" ) {
	net::NeuralNet network = net::NeuralNet(INPUTS, OUTPUTS, HIDDENS, NUM_IN_HIDDENS, ACTIVATION_FUNCTION);

	SECTION("Testing number of inputs") {
		REQUIRE(network.numberOfInputs() == INPUTS);
	}

	SECTION("Testing number of outputs") {
		REQUIRE(network.numberOfOutputs() == OUTPUTS);
	}

	SECTION("Testing number of hidden layers") {
		REQUIRE(network.numberOfHiddenLayers() == HIDDENS);
	}

	SECTION("Testing for correct output actication function") {
		REQUIRE(network.net[network.net.size() -1].getActivationFunctionName() == ACTIVATION_FUNCTION);
	}
}

TEST_CASE("Neural Network Changing", "[network]") {
	net::NeuralNet network = net::NeuralNet(INPUTS, OUTPUTS, HIDDENS, NUM_IN_HIDDENS, ACTIVATION_FUNCTION);
	std::vector<double> oldWeights = network.getWeights();
	std::vector< std::vector< std::vector<double> > > oldWeights3d = network.getWeights3D();
	std::vector<double> oldOutput = network.getOutput(input);

	SECTION("Randomizing weights") {
		network.randomizeWeights();

		REQUIRE(network.getWeights() != oldWeights);
		REQUIRE(network.getWeights3D() != oldWeights3d);
		REQUIRE(network.getOutput(input) != oldOutput);
	}
}

TEST_CASE("Neural Network Storing", "[network]") {
	net::NeuralNet network = net::NeuralNet(INPUTS, OUTPUTS, HIDDENS, NUM_IN_HIDDENS, ACTIVATION_FUNCTION);
	std::vector<double> oldWeights = network.getWeights();
	std::vector< std::vector< std::vector<double> > > oldWeights3d = network.getWeights3D();
	std::vector<double> oldOutput = network.getOutput(input);

	SECTION("Store into and load from file") {
		network.storeNet(FILENAME); // Store old
		net::NeuralNet fileNet = net::NeuralNet(FILENAME); // load new
		std::remove(FILENAME);

		REQUIRE(fileNet.getWeights() == oldWeights);
		REQUIRE(fileNet.getOutput(input) == oldOutput);

		std::vector< std::vector< std::vector<double> > > newWeights3d = fileNet.getWeights3D();
		for(int a = 0; a < oldWeights3d.size(); a++) {
			for(int b = 0; b < oldWeights3d[a].size(); b++) {
				REQUIRE(newWeights3d[a][b] == oldWeights3d[a][b]);
			}
		}
	}
}

TEST_CASE("Neural Network Copying", "[network]") {
	net::NeuralNet network = net::NeuralNet(INPUTS, OUTPUTS, HIDDENS, NUM_IN_HIDDENS, ACTIVATION_FUNCTION);
	std::vector<double> oldWeights = network.getWeights();
	std::vector< std::vector< std::vector<double> > > oldWeights3d = network.getWeights3D();
	std::vector<double> oldOutput = network.getOutput(input);

	net::NeuralNet newNet = net::NeuralNet(network);

	SECTION("Different weights") {
		REQUIRE(newNet.getWeights() == oldWeights);
		REQUIRE(newNet.getOutput(input) == oldOutput);

		std::vector< std::vector< std::vector<double> > > newWeights3d = network.getWeights3D();
		for(int a = 0; a < oldWeights3d.size(); a++) {
			for(int b = 0; b < oldWeights3d[a].size(); b++) {
				REQUIRE(newWeights3d[a][b] == oldWeights3d[a][b]);
			}
		}
	}

	SECTION("Testing number of inputs") {
		REQUIRE(newNet.numberOfInputs() == network.numberOfInputs());
	}

	SECTION("Testing number of outputs") {
		REQUIRE(newNet.numberOfOutputs() == network.numberOfOutputs());
	}

	SECTION("Testing number of hidden layers") {
		REQUIRE(newNet.numberOfHiddenLayers() == network.numberOfHiddenLayers());
	}
}
