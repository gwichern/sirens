#include "Loudness.h"

#include <string>
using namespace std;

#include "../support/math_support.h"

namespace Sirens {
	void Loudness::performCalculation() {
		value = 20 * log10(signal_rms(input));
	}

	string Loudness::toString() {
		return string("Loudness");
	}
}
