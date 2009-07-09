#include "Harmonicity.h"

#include <algorithm>
#include <cmath>
using namespace std;

#include "../support/math_support.h"

namespace Sirens {
	Harmonicity::Harmonicity(int history_size, int spectrum_size, int sample_rate) : Feature(history_size) {
		initialized = false;
		
		rawIndices.values = NULL;
		rawMagnitudes.values = NULL;
		accIndices.values = NULL;
		peakList.values = NULL;
			
		setSpectrumSize(spectrum_size);
		setSampleRate(sample_rate);
		
		setAbsThreshold(1.0);
		setThreshold(0.1);
		setSearchRegionLength(5);
		setMaxPeaks(3);
		setLPFCoefficient(0.7);
	}

	Harmonicity::~Harmonicity() {
		freeMemory();
	}

	void Harmonicity::setSpectrumSize(int spectrum_size) {
		spectrumSize = spectrum_size;
		
		initialized = false;
	}
	
	void Harmonicity::setSampleRate(int sample_rate) {
		sampleRate = sample_rate;
		
		initialized = false;
	}
	
	void Harmonicity::setAbsThreshold(double abs_threshold) {
		absThreshold = abs_threshold;
		
		initialized = false;
	}
	
	void Harmonicity::setThreshold(double new_threshold) {
		threshold = new_threshold;
		
		initialized = false;
	}
	
	void Harmonicity::setSearchRegionLength(int search_region_length) {
		searchRegionLength = search_region_length;
		
		initialized = false;
	}
	
	void Harmonicity::setMaxPeaks(int max_peaks) {
		maxPeaks = max_peaks;
		
		initialized = false;
	}
	
	void Harmonicity::setLPFCoefficient(double lpf_coefficient) {
		lpfCoefficient = lpf_coefficient;
		
		initialized = false;
	}
	
	
	int Harmonicity::getSpectrumSize() {
		return spectrumSize;
	}
	
	int Harmonicity::getSampleRate() {
		return sampleRate;
	}
	
	double Harmonicity::getAbsThreshold() {
		return absThreshold;
	}
	
	double Harmonicity::getThreshold() {
		return threshold;
	}
	
	int Harmonicity::getSearchRegionLength() {
		return searchRegionLength;
	}
	
	int Harmonicity::getMaxPeaks() {
		return maxPeaks;
	}
	
	double Harmonicity::getLPFCoefficient() {
		return lpfCoefficient;
	}
	
	void Harmonicity::initialize() {	
		// Minimum and maximum frequencies to consider.
		double min_hz = 90.0;
		double max_hz = 3500.0;
		
		minFrequencyIndex = int(ceil(min_hz * double(spectrumSize) / double(sampleRate)));
		maxFrequencyIndex = int(ceil(max_hz * double(spectrumSize) / double(sampleRate)));
		
		int vector_size = maxFrequencyIndex - minFrequencyIndex;
		
		// Parameters.
		searchRegionLength2 = (searchRegionLength - 1) / 2;
		nMax = 3;
		filterOldValue = 0;
		kVar = 0.01 / sqrt(2.0);
		
		// Vectors for peaks, magnitudes, and indices.
		rawIndices.size = 0;
		rawMagnitudes.size = 0;
		accIndices.size = 0;
		peakList.size = 0;
		
		rawIndices.values = new int[vector_size];
		rawMagnitudes.values = new double[vector_size];
		accIndices.values = new int[vector_size];
		peakList.values = new Peak[vector_size];
	}
	
	void Harmonicity::freeMemory() {
		if (rawIndices.values)
			delete [] rawIndices.values;
		
		if (rawMagnitudes.values)
			delete [] rawMagnitudes.values;
		
		if (accIndices.values)
			delete [] accIndices.values;
		
		if (peakList.values)
			delete [] peakList.values;
	}
	
	void Harmonicity::peakPick() {		
		for(int k = minFrequencyIndex + searchRegionLength2; k < maxFrequencyIndex - searchRegionLength2; k++) {
			double maxel = 0;
			
			for (int i = k - searchRegionLength2; i < k + searchRegionLength2 + 1; i++) {
				if (maxel < input->getValueAt(i))
					maxel = input->getValueAt(i);
			}
			
			if (input->getValueAt(k) >= maxel) {
				rawIndices.values[rawIndices.size] = k;
				rawIndices.size ++;
				rawMagnitudes.values[rawMagnitudes.size] = input->getValueAt(k);
				rawMagnitudes.size ++;
			}
		}
		
		double max_peak_mag = 0;
		
		for (int i = 0; i < rawMagnitudes.size + 1; i++) {
			if (max_peak_mag < rawMagnitudes.values[i])
				max_peak_mag = rawMagnitudes.values[i];
		}
		
		for (int k = 0; k < rawIndices.size; k++) {
			if ((input->getValueAt(rawIndices.values[k]) > threshold * max_peak_mag) && (input->getValueAt(rawIndices.values[k]) > absThreshold)) {
				accIndices.values[accIndices.size] = rawIndices.values[k];
				accIndices.size ++;
			}
		}
		
		for (int k = 0; k < accIndices.size; k++) {
			int ind = accIndices.values[k];
			
			double y1 = log(input->getValueAt(ind - 1));
			double y2 = log(input->getValueAt(ind));
			double y3 = log(input->getValueAt(ind + 1));
			
			double freq_bin_zero = (y1 - y3) / (2 * (y1 - 2 * y2 + y3));
			tempPeak.frequency = freq_bin_zero + ind;

			if (freq_bin_zero > 0) {
				double f = (y1 - y2) / (1 + 2 * freq_bin_zero);
				tempPeak.amplitude = y2 - f * freq_bin_zero * freq_bin_zero;
			} else {
				double f = (y3 - y2) / (1 - 2 * freq_bin_zero);
				tempPeak.amplitude = y2 - f * freq_bin_zero * freq_bin_zero;
			}

			tempPeak.frequency = sampleRate * tempPeak.frequency / input->getSize();
			tempPeak.amplitude = exp(tempPeak.amplitude);

			peakList.values[peakList.size] = tempPeak;
			peakList.size ++;
		}

		if (peakList.size > 1) {
			if (peakList.size > maxPeaks) {
				partial_sort(peakList.values, peakList.values + maxPeaks, peakList.values + peakList.size, SortAmplitudes);
				peakList.size = maxPeaks;
			}


			sort(peakList.values, peakList.values + peakList.size, SortFrequencies);

		}

		accIndices.size = rawIndices.size = rawMagnitudes.size = 0;
	}

	double Harmonicity::goldsteinGaussian(double x1, double x2, int n1, int n2, double f0, double k) {
		double mu1 = f0 * n1;
		double mu2 = f0 * n2;

		double sig1 = k * mu1;
		double sig2 = k * mu2;

		double sigmatinv1 = 1 / (sig1 * sig1);
		double sigmatinv2 = 1 / (sig2 * sig2);

		double xminmu1 = x1 - mu1;
		double xminmu2 = x2 - mu2;

		double con = 1 / ((2 * PI) * (sig1 * sig2));

		double p_f0 = con * exp(-0.5 * 
			(sigmatinv1 * (xminmu1 * xminmu1) + 
			sigmatinv2 * (xminmu2 * xminmu2)));

		return p_f0;
	}
	
	
	void Harmonicity::goldsteinCalc() {
		int p1, p2, n1, n2, pmax;
		double p0, f0, fmin, f1, f2, f1_rat, f2_rat, p_temp, f_temp;

		p0 = f0 = 0;
		fmin = peakList.values[0].frequency;
		pmax = peakList.size;

		for (p1 = 0; p1 < pmax - 1; p1++) {
			for (p2 = p1 + 1; p2 < pmax;  p2++) {
				f1 = peakList.values[p1].frequency;
				f2 = peakList.values[p2].frequency;

				for (n1 = 0; n1 < nMax - 1; n1++) {
					for (n2 = n1 + 1; n2 < nMax; n2++) {
						f1_rat = f1 / (n1 + 1.0);
						f2_rat = f2 / (n2 + 1.0);
						f_temp = ((f1_rat * f1_rat) + (f2_rat * f2_rat)) / (f1_rat + f2_rat);
						p_temp = goldsteinGaussian(f1, f2, n1 + 1, n2 + 1, f_temp, kVar);

						if ((p_temp > p0)) {
							f0 = f_temp;
							p0 = p_temp;
						}
					}
				}
			}
		}

		pitch = f0;
		value = p0;
	}
	
	void Harmonicity::performCalculation() {
		peakPick();

		if (peakList.size == 1) {
			pitch = peakList.values[0].frequency;
			value = 0;
		} else if (peakList.size == 0)
			pitch = value = 0;
		else
			goldsteinCalc();

		peakList.size = 0;
	}

	string Harmonicity::toString() {
		return "Harmonicity";
	}


	bool SortAmplitudes(const Peak& peak1, const Peak& peak2) {
		return peak1.amplitude > peak2.amplitude;
	}
	
	bool SortFrequencies(const Peak& peak1, const Peak& peak2) {
		return peak1.frequency < peak2.frequency;
	}
}
