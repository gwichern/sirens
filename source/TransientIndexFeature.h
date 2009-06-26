#ifndef TRANSIENTINDEXFEATURE_H
#define TRANSIENTINDEXFEATURE_H

#include "Feature.h"

namespace Sirens {
	class TransientIndexFeature : public Feature {
	private:
		double* mfccOld;
		double* mfccNew;
		double* dctMatrix;
		double* filterTemp;
		double* filterBank;
		
		int filters, mels, spectrumSize, sampleRate;
		
		void freeMemory();
		void initialize();
		
	public:
		TransientIndexFeature(int history_size = 1, int spectrum_size = 1024, int sample_rate = 44100, int in_filters = 15, int in_mels = 30);
		~TransientIndexFeature();
		
		void setSpectrumSize(int spectrum_size);
		void setSampleRate(int sample_rate);
		void setFilters(int in_filters);
		void setMels(int in_mels);
		
		int getSpectrumSize();
		int getSampleRate();
		int getFilters();
		int getMels();
		
		virtual void performCalculation();
		virtual string toString();
	};
}

#endif
