#ifndef HARMONICITY_H
#define HARMONICITY_H

#include "Feature.h"

namespace Sirens {
	struct Peak {
		double amplitude;
		double frequency;
	};
	
	struct HarmonicityDoubleVector {
		double* values;
		int size;
	};
	
	struct HarmonicityIntVector {
		int* values;
		int size;
	};
	
	struct HarmonicityPeakVector {
		Peak* values;
		int size;
	};
	
	class Harmonicity : public Feature {
	private:		
		double threshold, absThreshold;
		
		int searchRegionLength, searchRegionLength2;
		int maxPeaks, nMax;
		int sampleRate, spectrumSize;
		
		double kVar;
		
		int minFrequencyIndex, maxFrequencyIndex;
		
		HarmonicityIntVector rawIndices, accIndices;
		HarmonicityDoubleVector rawMagnitudes;
		HarmonicityPeakVector peakList;
		
		Peak tempPeak;
		
		double fftMax;
		double pitch;
		double lpfCoefficient;
		double filterOldValue;

		void peakPick();
		double goldsteinGaussian(double x1, double x2, int n1, int n2, double f0, double k);
		void goldsteinCalc();
		
		void freeMemory();
		void initialize();

	public:
		Harmonicity(int history_size = 1, int spectrum_size = 1024, int sample_rate = 44100);
		~Harmonicity();
		
		void setSpectrumSize(int spectrum_size);
		void setSampleRate(int sample_rate);
		void setAbsThreshold(double abs_threshold);
		void setThreshold(double new_threshold);
		void setSearchRegionLength(int search_region_length);
		void setMaxPeaks(int max_peaks);
		void setLPFCoefficient(double lpf_coefficient);
		
		int getSpectrumSize();
		int getSampleRate();
		double getAbsThreshold();
		double getThreshold();
		int getSearchRegionLength();
		int getMaxPeaks();
		double getLPFCoefficient();
		
		virtual void performCalculation();
		virtual string toString();
	};
	
	// Callbacks for sorting peaks.
	bool SortAmplitudes(const Peak& peak1, const Peak& peak2);
	bool SortFrequencies(const Peak& peak1, const Peak& peak2);
}

#endif
