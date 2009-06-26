#include "TransientIndexFeature.h"

#include "math_support.h"

#include <cmath>
using namespace std;

namespace Sirens {
	TransientIndexFeature::TransientIndexFeature(int history_size, int spectrum_size, int sample_rate, int in_filters, int in_mels) : Feature(history_size) {
		initialized = false;
		
		mfccOld = NULL;
		mfccNew = NULL;
		dctMatrix = NULL;
		filterTemp = NULL;
		filterBank = NULL;
			
		setSpectrumSize(spectrum_size);
		setSampleRate(sample_rate);
		setFilters(in_filters);
		setMels(in_mels);
	}
	
	TransientIndexFeature::~TransientIndexFeature() {
		freeMemory();
	}
	
	void TransientIndexFeature::freeMemory() {
		if (dctMatrix)
			delete dctMatrix;
		
		if (filterBank)
			delete filterBank;
		
		if (filterTemp)
			delete filterTemp;
		
		if (mfccNew)
			delete mfccNew;
		
		if (mfccOld)
			delete mfccOld;
	}
	
	void TransientIndexFeature::initialize() {	
		dctMatrix = new double[filters * mels];
		filterBank = new double[filters * spectrumSize];
		filterTemp = new double[filters];
	
		mfccNew = new double[mels];
		mfccOld = new double[mels];
	
		// Initialisation
		double min_mel = hz_to_mel(50.0);
		double max_mel = hz_to_mel(sampleRate / 2.0);
	
		double* filter_values = new double[spectrumSize];
		double* filter_centers = new double[filters + 2];
	
		for (int i = 0; i < spectrumSize; i++)
			filter_values[i] = double(sampleRate * i) / double(2 * (spectrumSize - 1));

		for (int i = 0; i < mels; i++) {
			for (int j = 0; j < filters; j++)
				dctMatrix[i * filters + j] = cos((i + 1) * (PI / filters * (j + 0.5)));
		}
	
		for (int i = 0; i < filters + 2; i++)
			filter_centers[i] = mel_to_hz(min_mel + ((max_mel - min_mel) / (filters + 1)) * i);
		
		for (int i = 0; i < filters; i++) {
			for (int j = 0; j < spectrumSize; j++) {
				if (filter_values[j] >= filter_centers[i] && filter_values[j] < filter_centers[i + 1])
					filterBank[(i * spectrumSize) + j] = (filter_values[j] - filter_centers[i]) / (filter_centers[i + 1] - filter_centers[i]);
				else if (filter_values[j] >= filter_centers[i + 1] && filter_values[j] < filter_centers[i + 2])
					filterBank[(i * spectrumSize) + j] = (filter_values[j] - filter_centers[i + 2]) / (filter_centers[i + 1] - filter_centers[i + 2]);
				else
					filterBank[(i * spectrumSize) + j] = 0;
			}
		}
	
		for (int i = 0; i < mels; i++) {
			mfccNew[i] = 0;
			mfccOld[i] = 0;
		}
	
		for (int i = 0; i < filters; i++)
			filterTemp[i] = 0;
	
		delete[] filter_values;
		delete[] filter_centers;
	}
 
	void TransientIndexFeature::setSpectrumSize(int spectrum_size) {
		spectrumSize = spectrum_size;
		
		initialized = false;
	}
	
	void TransientIndexFeature::setSampleRate(int sample_rate) {
		sampleRate = sample_rate;
		
		initialized = false;
	}
	
	void TransientIndexFeature::setFilters(int in_filters) {
		filters = in_filters;
		
		initialized = false;
	}
	
	void TransientIndexFeature::setMels(int in_mels) {
		mels = in_mels;
		
		initialized = false;
	}
	
	int TransientIndexFeature::getSpectrumSize() {
		return spectrumSize;
	}
	
	int TransientIndexFeature::getSampleRate() {
		return sampleRate;
	}
	
	int TransientIndexFeature::getFilters() {
		return filters;
	}
	
	int TransientIndexFeature::getMels() {
		return mels;
	}
	
	void TransientIndexFeature::performCalculation() {
		int spectrum_size = input->getSize();
		
		// Calculate the MFCC vector for the current frame.
		for (int i = 0; i < filters; i++) {
			filterTemp[i] = 0;
		
			for (int j = 0; j < spectrum_size; j++)
				filterTemp[i] += filterBank[(i * spectrum_size) + j] * input->getValueAt(j);
			
			filterTemp[i] = (filterTemp[i] > 0) ? log(filterTemp[i]) : 0;
		}
	
		for (int i = 0; i < mels; i++) {
			mfccNew[i] = 0;
		
			for (int j = 0; j < filters; j++)
				mfccNew[i] += dctMatrix[(i * filters) + j] * filterTemp[j];
		}
	
		// Calculate transient index.
		double sum_of_squared_error = 0;
	
		for (int i = 0; i < mels; i++) {
			double error = mfccNew[i] - mfccOld[i];
			sum_of_squared_error += error * error;
		}
		
		value = sqrt(sum_of_squared_error);
	
		// Swap MFCC vectors.
		double* mfccTemp = mfccOld;
		mfccOld = mfccNew;
		mfccNew = mfccTemp;
	}
	
	string TransientIndexFeature::toString() {
		return "Transient Index";
	}
}
