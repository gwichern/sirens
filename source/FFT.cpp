/*
	Copyright 2008, 2009 Brandon Mechtley
	
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

#include "FFT.h"

namespace Sirens {
	FFT::FFT(int fft_size, double* input) {
		fftSize = fft_size;
		output = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * getOutputSize());
		plan = fftw_plan_dft_r2c_1d(getInputSize(), input, output, FFTW_ESTIMATE);
	}
	
	FFT::~FFT() {
		fftw_free(output);
		fftw_destroy_plan(plan);
	}
	
	void FFT::calculate() {
		fftw_execute(plan);
	}
	
	fftw_complex* FFT::getOutput() {
		return output;
	}
	
	int FFT::getInputSize() {
		return fftSize;
	}
	
	int FFT::getOutputSize() {
		return getInputSize() / 2 + 1;
	}
}
