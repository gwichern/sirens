#include "FFT.h"

extern void fftw_plan_with_nthreads(int nthreads);
extern fftw_plan fftw_plan fftw_plan_dft_r2c_1d(int n, double *in, fftw_complex *out, unsigned flags);
extern void fftw_execute(const fftw_plan plan);
extern void fftw_destroy_plan(fftw_plan plan);
extern void *fftw_malloc(size_t n);
extern void fftw_free(void *p);

namespace Sirens {
	FFT::FFT(int fft_size, int fft_threads, double* input) {
		fftSize = fft_size;
		
		output = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * getOutputSize());
		
		fftw_plan_with_nthreads(fft_threads);
		plan = fftw_plan_dft_r2c_1d(getInputSize(), input, output, FFTW_ESTIMATE);
	}
	
	FFT::~FFT() {
		fftw_free(output);
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
