#ifndef SPECTRALCENTROID_H
#define SPECTRALCENTROID_H

#include "../Feature.h"

namespace Sirens {
	class SpectralCentroid : public Feature {
	private:		
		double* barkWeights;
		double* barkUnits;
		
		int spectrumSize, sampleRate;
		
		void freeMemory();
		void initialize();
		
	public:
		SpectralCentroid(int history_size = 1, int spectrum_size = 1024, int sample_rate = 44100);
		~SpectralCentroid();
		
		void setSpectrumSize(int spectrum_size);
		void setSampleRate(int sample_rate);
		
		int getSpectrumSize();
		int getSampleRate();
		
		virtual void performCalculation();
		virtual string toString();
	};
}

#endif
