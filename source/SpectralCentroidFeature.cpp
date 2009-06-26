#include "SpectralCentroidFeature.h"

#include "math_support.h"

namespace Sirens {
	SpectralCentroidFeature::SpectralCentroidFeature(int history_size, int spectrum_size, int sample_rate) : Feature(history_size) {
		initialized = false;
		
		barkWeights = NULL;
		barkUnits = NULL;
		
		setSpectrumSize(spectrum_size);
		setSampleRate(sample_rate);
	}

	SpectralCentroidFeature::~SpectralCentroidFeature() {
		freeMemory();
	}
	
	void SpectralCentroidFeature::freeMemory() {
		if (barkUnits)
			delete [] barkUnits;
		
		if (barkWeights)
			delete [] barkWeights;
	}
	
	void SpectralCentroidFeature::initialize() {
		barkUnits = new double[spectrumSize];
		barkWeights = new double[spectrumSize - 1];
		
		for (int i = 0; i < spectrumSize; i++)
			barkUnits[i] = hz_to_bark(double(sampleRate * i) / double(2 * (spectrumSize - 1)));
		
		for (int i = 0; i < spectrumSize - 1; i++)
			barkWeights[i] = barkUnits[i + 1] - barkUnits[i];
		
		initialized = true;
	}
	
	void SpectralCentroidFeature::setSpectrumSize(int spectrum_size) {
		spectrumSize = spectrum_size;
		
		initialized = false;
	}
	
	void SpectralCentroidFeature::setSampleRate(int sample_rate) {
		sampleRate = sample_rate;
		
		initialized = false;
	}
	
	int SpectralCentroidFeature::getSpectrumSize() {
		return spectrumSize;
	}
	
	int SpectralCentroidFeature::getSampleRate() {
		return sampleRate;
	}
	
	void SpectralCentroidFeature::performCalculation() {
		double sum = 0;
		value = 0;
	
		int size = input->getSize();
	
		double* input_item = input->getData() + 1;
		double* weight_item = barkWeights;
				
		for (int i = 1; i < size; i++) {
			sum += (*input_item) * (*input_item) * (*weight_item);
			
			input_item ++;
			weight_item ++;
		}
		
		input_item = input->getData() + 1;
		weight_item = barkWeights;
		double* unit_item = barkUnits + 1;
		
		for (int i = 1; i < size; i++) {
			value += (*input_item) * (*input_item) * ((*weight_item) / sum) * (*unit_item);
			
			input_item ++;
			unit_item ++;
			weight_item ++;
		}
	}

	string SpectralCentroidFeature::toString() {
		return "Spectral Centroid";
	}
}
