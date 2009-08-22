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
