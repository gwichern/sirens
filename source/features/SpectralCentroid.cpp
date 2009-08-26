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

#include "SpectralCentroid.h"

#include "../support/math_support.h"

namespace Sirens {
	SpectralCentroid::SpectralCentroid(int history_size, int spectrum_size, int sample_rate) : Feature(history_size) {
		initialized = false;
		
		barkWeights = NULL;
		barkUnits = NULL;
		
		setSpectrumSize(spectrum_size);
		setSampleRate(sample_rate);
	}

	SpectralCentroid::~SpectralCentroid() {
		freeMemory();
	}
	
	void SpectralCentroid::freeMemory() {
		if (barkUnits)
			delete [] barkUnits;
		
		if (barkWeights)
			delete [] barkWeights;
	}
	
	void SpectralCentroid::initialize() {
		barkUnits = new double[spectrumSize];
		barkWeights = new double[spectrumSize - 1];
		
		for (int i = 0; i < spectrumSize; i++)
			barkUnits[i] = hz_to_bark(double(sampleRate * i) / double(2 * (spectrumSize - 1)));
		
		for (int i = 0; i < spectrumSize - 1; i++)
			barkWeights[i] = barkUnits[i + 1] - barkUnits[i];
		
		initialized = true;
	}
	
	void SpectralCentroid::setSpectrumSize(int spectrum_size) {
		spectrumSize = spectrum_size;
		
		initialized = false;
	}
	
	void SpectralCentroid::setSampleRate(int sample_rate) {
		sampleRate = sample_rate;
		
		initialized = false;
	}
	
	int SpectralCentroid::getSpectrumSize() {
		return spectrumSize;
	}
	
	int SpectralCentroid::getSampleRate() {
		return sampleRate;
	}
	
	void SpectralCentroid::performCalculation() {
		double sum = 0;
		value = 0;
		
		int size = input->getSize();
		double* weight_item = barkWeights;
		double* input_item = input->getData() + 1;
		
		for (int i = 1; i < size; i++) {
			sum += (*input_item) * (*input_item) * (*weight_item);
			
			weight_item ++;
			input_item ++;
		}
		
		weight_item = barkWeights;
		double* unit_item = barkUnits + 1;
		input_item = input->getData() + 1;
		
		for (int i = 1; i < size; i++) {
			value += (*input_item) * (*input_item) * ((*weight_item) / sum) * (*unit_item);
			
			unit_item ++; 
			weight_item ++;
			input_item ++;
		}		
	}

	string SpectralCentroid::toString() {
		return "Spectral Centroid";
	}
}
