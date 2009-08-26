/*
	Copyright 2007, 2009 Brandon Mechtley, Gordon Wichern, Alex Fink, and Harvey Thornburg
	
	This file is part of Sirens.
	
	Sirens is free software: you can redistribute it and/or modify it under the terms 
	of the GNU Lesser General Public License as  published by the Free Software 
	Foundation, either version 3 of the License, or (at your option) any later version.
	
	Sirens is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
	PURPOSE.  See the GNU General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License along
	with Sirens. If not, see <http://www.gnu.org/licenses/>.
*/

#include "TransientIndex.h"

#include "../support/math_support.h"

#include <cmath>
using namespace std;

namespace Sirens {
	TransientIndex::TransientIndex(int history_size, int spectrum_size, int sample_rate, int in_filters, int in_mels) : Feature(history_size) {
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
	
	TransientIndex::~TransientIndex() {
		freeMemory();
	}
	
	void TransientIndex::freeMemory() {
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
	
	void TransientIndex::initialize() {	
		dctMatrix = new double[filters * mels];
		filterBank = new double[filters * spectrumSize];
		filterTemp = new double[filters];
		
		mfccNew = new double[mels];
		mfccOld = new double[mels];
	
		// Initialisation
		double min_mel = hz_to_mel(50.0);
		double max_mel = hz_to_mel(sampleRate / 2);
		
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
 
	void TransientIndex::setSpectrumSize(int spectrum_size) {
		spectrumSize = spectrum_size;
		
		initialized = false;
	}
	
	void TransientIndex::setSampleRate(int sample_rate) {
		sampleRate = sample_rate;
		
		initialized = false;
	}
	
	void TransientIndex::setFilters(int in_filters) {
		filters = in_filters;
		
		initialized = false;
	}
	
	void TransientIndex::setMels(int in_mels) {
		mels = in_mels;
		
		initialized = false;
	}
	
	int TransientIndex::getSpectrumSize() {
		return spectrumSize;
	}
	
	int TransientIndex::getSampleRate() {
		return sampleRate;
	}
	
	int TransientIndex::getFilters() {
		return filters;
	}
	
	int TransientIndex::getMels() {
		return mels;
	}
	
	void TransientIndex::performCalculation() {
		int spectrum_size = input->getSize();
		
		// Calculate the MFCC vector for the current frame.
		for (int i = 0; i < filters; i++) {
			filterTemp[i] = 0;
			
			double* input_item = input->getData();
			
			for (int j = 0; j < spectrum_size; j++) {
				filterTemp[i] += filterBank[(i * spectrum_size) + j] * (*input_item);
				input_item ++;
			}
			
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
	
	string TransientIndex::toString() {
		return "Transient Index";
	}
}
