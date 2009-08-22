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
