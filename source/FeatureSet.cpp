#include "FeatureSet.h"

#include <fstream>
using namespace std;

namespace Sirens {
	void FeatureSet::addSampleFeature(Feature* feature) {
		sampleFeatures.push_back(feature);
		features.push_back(feature);
	}

	void FeatureSet::addSpectralFeature(Feature* feature) {
		spectralFeatures.push_back(feature);
		features.push_back(feature);
	}
	
	vector<Feature*> FeatureSet::getFeatures() {
		return features;
	}
	
	vector<Feature*> FeatureSet::getSampleFeatures() {
		return sampleFeatures;
	}

	vector<Feature*> FeatureSet::getSpectralFeatures() {
		return spectralFeatures;
	}
	
	void FeatureSet::setSampleFeatures(vector<Feature*> sample_features) {
		sampleFeatures = sample_features;
	}
	
	void FeatureSet::setSpectralFeatures(vector<Feature*> spectral_features) {
		spectralFeatures = spectral_features;
	}
	
	int FeatureSet::getMinHistorySize() {
		int min_history_size = -1;
		
		vector<Feature*> features = getFeatures();
		
		for (int i = 0; i < features.size(); i++) {
			if (min_history_size < 0 || features[i]->getHistorySize() < min_history_size)
				min_history_size = features[i]->getHistorySize();
		}
		
		return min_history_size;
	}
	
	void FeatureSet::saveCSV(string csv_path) {
		fstream file(csv_path.c_str(), ios::out);
		
		vector<Feature*> features = getFeatures();
		
		for (int i = 0; i < getMinHistorySize(); i++) {
			for (unsigned int j = 0; j < features.size(); j++)
				file << features[j]->getHistoryFrame(i) << ",";
	
			file << i << endl;
		}

		file.close();
	}

	void FeatureSet::calculateSampleFeatures(CircularArray* sample_array) {	
		for (unsigned int j = 0; j < sampleFeatures.size(); j++)
			sampleFeatures[j]->calculate(sample_array);
	
		for (unsigned int j = 0; j < sampleFeatures.size(); j++)
			sampleFeatures[j]->waitForCompletion();
	}

	void FeatureSet::calculateSpectralFeatures(CircularArray* spectrum_array) {
		for (unsigned int j = 0; j < spectralFeatures.size(); j++)
			spectralFeatures[j]->calculate(spectrum_array);
	
		for (unsigned int j = 0; j < spectralFeatures.size(); j++)
			spectralFeatures[j]->waitForCompletion();
	}
}
