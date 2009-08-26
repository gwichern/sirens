/*
	Copyright 2007, 2009 Brandon Mechtley, Gordon Wichern, Alex Fink, and Harvey Thornburg

	This file is part of Sirens.

	Sirens is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Sirens is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Sirens.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HARMONICITY_H
#define HARMONICITY_H

#include "../Feature.h"

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
		int fftSize;
		
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

		void pickPeaks();
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
