#include "Interpolator.h"

#include "WireFitInterpolator.h"

using namespace net;

Interpolator * Interpolator::getAnyInterpolatorFromFile(std::ifstream *in) {
	int placeInFile = in->tellg();
	Interpolator *interpolator;

	interpolator = (Interpolator *)new WireFitInterpolator();
	if(interpolator->initFromFile(in)) {
		return interpolator;
	}
	delete interpolator;
	in->seekg(placeInFile);

	std::cout << "Could not get any interpolator from file\n";
	throw 1;
};