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