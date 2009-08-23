#ifndef __FEATURESET_H__
#define __FEATURESET_H__

#include "Feature.h"
#include "CircularArray.h"

/*
	FeatureSet - contains multiple features that are calculated on either 
		a) raw sample data from a frame of audio or b) spectrum data from 
		performing an FFT on a frame of audio. Wraps calculation routines
		so that they can be run in bulk.
*/
 
namespace Sirens {
	class FeatureSet {
	private:
		vector<Feature*> features;				// Vector of all features for non-segregated operations.
		vector<Feature*> sampleFeatures;
		vector<Feature*> spectralFeatures;
		
	public:
		void startFeatures();					// Spawns running threads for all features.
		void stopFeatures();					// Signals all features to stop. Necessary before destroying the features.
		
		void addSampleFeature(Feature* feature);
		void addSpectralFeature(Feature* feature);
		
		vector<Feature*> getFeatures();
		vector<Feature*> getSampleFeatures();
		vector<Feature*> getSpectralFeatures();
		
		void setSampleFeatures(vector<Feature*> sample_features);
		void setSpectralFeatures(vector<Feature*> spectral_features);
		
		int getMinHistorySize();
		
		void saveCSV(string csv_path);			// Saves a CSV file containing the features' trajectories.
		
		void calculateSampleFeatures(CircularArray* sample_array);
		void calculateSpectralFeatures(CircularArray* spectrum_array);
	};
}

#endif