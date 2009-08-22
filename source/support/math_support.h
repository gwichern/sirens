/*
 *  math_support.h
 *  SIRENS
 *
 *  Created by Brandon Mechtley on 7/27/07.
 *  Copyright 2007 Brandon Mechtley. All rights reserved.
 *
 */

#ifndef MATH_SUPPORT_H
#define MATH_SUPPORT_H

#include <cmath>
using namespace std;

namespace Sirens { 
	const double pi = 2 * asin(1.0);
	
	// General math functions.
	int minimum(int a, int b);
	int maximum(int a, int b);
	double minimum(double a, double b);
	double maximum(double a, double b);

	int next_pow(int n, int base);

	double signal_rms(double* data, int size);

	double lerp(double value, double min, double max);
	
	// Perceptual scale conversion.
	double hz_to_bark(double hz);
	double bark_to_hz(double bark);
	double hz_to_mel(double hz);
	double mel_to_hz(double mel);

	double* create_hamming_window(int size);
}

#endif
