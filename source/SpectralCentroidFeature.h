#ifndef SPECTRALCENTROIDFEATURE_H
#define SPECTRALCENTROIDFEATURE_H

#include "Feature.h"

namespace Sirens {
	class SpectralCentroidFeature : public Feature {
	private:		
		double* barkWeights;
		double* barkUnits;
		
		int spectrumSize, sampleRate;
		
		void freeMemory();
		void initialize();
		
	public:
		SpectralCentroidFeature(int history_size = 1, int spectrum_size = 1024, int sample_rate = 44100);
		~SpectralCentroidFeature();
		
		void setSpectrumSize(int spectrum_size);
		void setSampleRate(int sample_rate);
		
		int getSpectrumSize();
		int getSampleRate();
		
		virtual void performCalculation();
		virtual string toString();
	};
}

#endif