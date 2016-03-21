#include <cstdio>
#include "catch.h"
#include "../Software/NeuralNet.h"

// Constants set for testing, could be anything
const int INPUTS(4);
const int OUTPUTS(2);
const int HIDDENS(3);
const int NUM_IN_HIDDENS(6);
const char * FILENAME = "dasgfgjhasdgkfhjkgfjaskfgh.dat";
const char * ACTIVATION_FUNCTION("sigmoid");
std::vector<double> input = {3, 5, 2, 5};

TEST_CASE( "Neural Network Sizing", "[network]" ) {
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
		REQUIRE(fileNet.getWeights3D() == oldWeights3d);
		REQUIRE(fileNet.getOutput(input) == oldOutput);
	}
}

TEST_CASE("Neural Network Copying", "[network]") {
	net::NeuralNet network = net::NeuralNet(INPUTS, OUTPUTS, HIDDENS, NUM_IN_HIDDENS, ACTIVATION_FUNCTION);
	std::vector<double> oldWeights = network.getWeights();
	std::vector< std::vector< std::vector<double> > > oldWeights3d = network.getWeights3D();
	std::vector<double> oldOutput = network.getOutput(input);

	net::NeuralNet newNet = net::NeuralNet(network);

	SECTION("Different weights") {
		REQUIRE(newNet.getWeights() != oldWeights);
		REQUIRE(newNet.getWeights3D() != oldWeights3d);
		REQUIRE(newNet.getOutput(input) != oldOutput);
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
