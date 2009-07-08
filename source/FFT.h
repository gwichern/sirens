#ifndef FFT_H
#define FFT_H

#include <fftw3.h>

extern void fftw_plan_with_nthreads(int nthreads);
extern fftw_plan fftw_plan_dft_r2c_1d(int n, double *in, fftw_complex *out, unsigned flags);
extern void fftw_execute(const fftw_plan plan);
extern void fftw_destroy_plan(fftw_plan plan);
extern void *fftw_malloc(size_t n);
extern void fftw_free(void *p);

namespace Sirens {
	class FFT {
	private:
		fftw_complex* output;
		fftw_plan plan;
		
		int fftSize;
		
	public:
		FFT(int fft_size, int fft_threads, double* input);
		~FFT();
		
		void calculate();
		
		int getInputSize();
		int getOutputSize();
		
		fftw_complex* getOutput();
	};
}

#endif
