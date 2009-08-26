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

#ifndef MATH_SUPPORT_H
#define MATH_SUPPORT_H

#include <cmath>
using namespace std;

#include "../CircularArray.h"

namespace Sirens { 
	const double PI = 2 * asin(1.0);
	
	// General math functions.
	int minimum(int a, int b);
	int maximum(int a, int b);
	double minimum(double a, double b);
	double maximum(double a, double b);

	int next_pow(int n, int base);

	double signal_rms(CircularArray* input);

	double lerp(double value, double min, double max);
	
	// Perceptual scale conversion.
	double hz_to_bark(double hz);
	double bark_to_hz(double bark);
	double hz_to_mel(double hz);
	double mel_to_hz(double mel);

	double* create_hamming_window(int size);
}

#endif
