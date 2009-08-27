/*
	Copyright 2007, 2008, 2009 Brandon Mechtley
	
	This file is part of Sirens.
	
	Sirens is free software: you can redistribute it and/or modify it under the terms 
	of the GNU Lesser General Public License as  published by the Free Software 
	Foundation, either version 3 of the License, or (at your option) any later version.
	
	Sirens is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
	PURPOSE.  See the GNU General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License along
	with Sirens. If not, see <http://www.gnu.org/licenses/>.
*/

#include <cmath>
using namespace std;

#include "math_support.h"

namespace Sirens {
	int round(double a) {
		if ((a - floor(a)) >= 0.5)
			return ceil(a);
		else
			return floor(a);
	}
	
	int minimum(int a, int b) {
		return a < b ? a : b;
	}

	int maximum(int a, int b) {
		return a > b ? a : b;
	}

	double minimum(double a, double b) {
		return a < b ? a : b;
	}

	double maximum(double a, double b) {
		return a > b ? a : b;
	}

	
	double akaike(int k, int n, double rss) {
		double aic = 93 * k + n * log(rss / n);
		return aic + (double(2 * k * (k + 1)) / double(n - k - 1));
	}
	
	int next_pow(int n, int base) {
		int j = base;

		while (j < n)
			j *= base;

		return j;
	}

	double signal_rms(CircularArray* input) {
		double sum_of_squares = 0;
		
		double* data = input->getData();
		
		for (int i = 0; i < input->getSize(); i++) {
			sum_of_squares += (*data) * (*data);
			data ++;
		}
		
		return sqrt(sum_of_squares / (double)input->getSize());
	}

	double hz_to_bark(double hz) {
		return 6.0 * asinh(hz / 600.0);
	}

	double bark_to_hz(double bark) {
		return 600.0 * sinh(bark / 6.0);
	}

	double hz_to_mel(double hz) {
		return 2595.0 * log10((hz / 700.0) + 1.0);
	}

	double mel_to_hz(double mel) {
	    return 700.0 * (pow(10.0, (mel / 2595.0)) - 1.0);
	}

	double* create_hamming_window(int size) {
		double* window = new double[size];
	
		for (int i = 0; i < size; i++)
			window[i] = 0.54 - 0.46 * cos((2 * PI * i) / (size - 1));
	
		return window;
	}
	
	double lerp(double value, double min, double max) {
		return (value - min) / (max - min);
	}
}
