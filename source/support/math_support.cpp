/*
 *  math_support.cpp
 *  SIRENS
 *
 *  Created by Brandon Mechtley on 7/27/07.
 *  Copyright 2007 Brandon Mechtley. All rights reserved.
 *
 */

#include <cmath>
using namespace std;

#include "math_support.h"

namespace Sirens {
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
