#include "LinearInterpolator.h"

#include <string>

using namespace net;

LinearInterpolator::LinearInterpolator() {

}

bool LinearInterpolator::initFromFile(std::ifstream *in) {
	std::string name;
	if (!(*in >> name)) {
		std::cout << "Interpolator could not read ifstrem\n";
		throw 1;
	}

	if (name != "Linear") {
		return false;
	}

	return true;
}

void LinearInterpolator::storeInterpolator(std::ofstream *out) {
	*out << "Linear\n";
}
