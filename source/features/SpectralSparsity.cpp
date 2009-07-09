#include "SpectralSparsity.h"

#include <string>
using namespace std;

#include "../support/math_support.h"

namespace Sirens {
	void SpectralSparsity::performCalculation() {
		double max = 0;
		double sum = 0;
		
		double* input_item = input->getData();
		int size = input->getSize();
		
		for (int i = 0; i < size; i++) {
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
