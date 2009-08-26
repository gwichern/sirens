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

#ifndef FFT_H
#define FFT_H

#include <fftw3.h>

namespace Sirens {
	class FFT {
	private:
		fftw_complex* output;
		fftw_plan plan;
		int fftSize;
		
	public:
		FFT(int fft_size, double* input);
		~FFT();
		
		void calculate();
		
		int getInputSize();
		int getOutputSize();
		
		fftw_complex* getOutput();
	};
}

#endif
