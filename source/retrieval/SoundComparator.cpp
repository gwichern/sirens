/*
	Copyright 2007, 2008, 2009 Brandon Mechtley, Gordon Wichern, and Jiachen Xue
	
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

#include "SoundComparator.h"

namespace Sirens {
	SoundComparator::SoundComparator(FeatureSet* features_in) {
		setFeatures(features_in);
	}

	SoundComparator::~SoundComparator() {
		freeMemory();
	}
	
	void SoundComparator::freeMemory() {
		for (int i = 0; i < featureComparators.size(); i++)
			delete featureComparators[i];	
	}
	
	void SoundComparator::setFeatures(FeatureSet* features_in) {
		features = features_in;
		
		// Make new comparators for every feature in the feature set.
		if (features != NULL) {
			freeMemory();
			
			vector<Feature*> feature_list = features->getFeatures();

			featureComparators.resize(feature_list.size());

			for (int i = 0; i < feature_list.size(); i++)
				featureComparators[i] = new FeatureComparator(feature_list[i]);
		}
	}
	
	FeatureSet* SoundComparator::getFeatures() {
		return features;
	}

	vector<FeatureComparator*> SoundComparator::getFeatureComparators() {
		return featureComparators;
	}

	double SoundComparator::compare(SoundComparator* sound_comparator) {
		vector<FeatureComparator*> other_comparators = sound_comparator->getFeatureComparators();
		
		if (other_comparators.size() != featureComparators.size())
			return 0;
		else {
			double likelihood = 0;
			
			// Compare each feature against corresponding features from the other sound; sum likelihood.
			for (int i = 0; i < featureComparators.size(); i++)
				likelihood += featureComparators[i]->compare(other_comparators[i]);
			
			return likelihood;
		}
	}
	
	// Initialize HMMs, curve parameters.
	void SoundComparator::initialize() {
		for (int i = 0; i < featureComparators.size(); i++)
			featureComparators[i]->initialize();
	}
}
