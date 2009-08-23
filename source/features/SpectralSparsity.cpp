#include "SpectralSparsity.h"

#include <string>
using namespace std;

#include "../support/math_support.h"

namespace Sirens {
	void SpectralSparsity::performCalculation() {
		double max = 0;
		double sum = 0;
		
		int input_size = input->getSize();
		double* input_item = input->getData();
		
		for (int i = 0; i < input_size; i++) {
			max = maximum((*input_item), max);
			sum += (*input_item);
			
			input_item ++;
		}
		
		value = max / sum;
	}

	string SpectralSparsity::toString() {
		return string("Spectral Sparsity");
	}
}
