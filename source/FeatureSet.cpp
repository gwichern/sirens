/*
	Copyright 2009 Brandon Mechtley
	
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

#include "FeatureSet.h"

#include "support/string_support.h"

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
		vector<vector<double> > trajectories;
		
		vector<Feature*> features = getFeatures();
		
		for (int i = 0; i < getMinHistorySize() - 1; i++) {
			vector<double> row;
			
			for (unsigned int j = 0; j < features.size(); j++)
				row.push_back(features[j]->getHistoryFrame(i));
			
			trajectories.push_back(row);
		}
		
		write_csv_file(csv_path, trajectories);
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
