#ifndef __FEATURESET_H__
#define __FEATURESET_H__

#include "Feature.h"
#include "CircularArray.h"

namespace Sirens {
	class FeatureSet {
	private:
		vector<Feature*> features;
		vector<Feature*> sampleFeatures;
		vector<Feature*> spectralFeatures;
		
	public:
		void addSampleFeature(Feature* feature);
		void addSpectralFeature(Feature* feature);
		
		vector<Feature*> getFeatures();
		vector<Feature*> getSampleFeatures();
		vector<Feature*> getSpectralFeatures();
		
		void setSampleFeatures(vector<Feature*> sample_features);
		void setSpectralFeatures(vector<Feature*> spectral_features);
		
		int getMinHistorySize();
		
		void saveCSV(string csv_path);
		
		void calculateSampleFeatures(CircularArray* sample_array);
		void calculateSpectralFeatures(CircularArray* spectrum_array);
	};
}

#endif