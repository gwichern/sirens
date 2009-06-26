#include "LoudnessFeature.h"

#include <string>
using namespace std;

#include "math_support.h"

namespace Sirens {
	void LoudnessFeature::performCalculation() {
		value = 20 * log10(signal_rms(input->getData(), input->getSize()));
	}

	string LoudnessFeature::toString() {
		return string("Loudness");
	}
}
