/*
	Copyright 2007, 2009 Brandon Mechtley, Gordon Wichern, Alex Fink, and Harvey Thornburg

	This file is part of Sirens.

	Sirens is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Sirens is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Sirens.  If not, see <http://www.gnu.org/licenses/>.
*/

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
