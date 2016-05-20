#include "../include/Interpolator.h"

#include <assert.h>
#include <iostream>

#include "../include/LSInterpolator.h"

using namespace rl;

Interpolator * Interpolator::getAnyInterpolatorFromFile(std::ifstream *in) {
	long placeInFile = in->tellg();
	Interpolator *interpolator;

	interpolator = static_cast<Interpolator *>(new LSInterpolator());
	if(interpolator->initFromStream(in)) {
		return interpolator;
	}
	delete interpolator;
	in->seekg(placeInFile);
	assert(false);
};
