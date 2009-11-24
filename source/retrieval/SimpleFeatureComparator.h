/*
	Copyright 2009 Brandon Mechtley and Gordon Wichern
	
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

#ifndef __SIMPLEFEATURECOMPARATOR_H__
#define __SIMPLEFEATURECOMPARATOR_H__

#include "../Feature.h"

namespace Sirens {
	
	// Class that creates a simple single Gaussian non-dynamic retrieval model for an individual feature trajectory.
	class SimpleFeatureComparator {
	private:
		Feature* feature;							// feature trajectory that this model represents.
		bool initialized;
		
		// Gaussian Model
		double featureMean;
		double featureVariance;
		
		// Initialization.
		void fitGaussian();							// compute the mean and variance of the feature trajectory
	
	public:
		SimpleFeatureComparator(Feature* feature_in);
		~SimpleFeatureComparator();
		
		// General attributes.
		Feature* getFeature();
		bool isInitialized();
		
		// Gaussian attributes.
		double getMean();
		double getVariance();
		
		// Operations.
		void initialize();
		double compare(SimpleFeatureComparator* model);
	};
}

#endif
