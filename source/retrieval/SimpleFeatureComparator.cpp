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

#include "SimpleFeatureComparator.h"
#include "../support/math_support.h"
#include <cmath>
#include <limits>
using namespace std;

namespace Sirens {
	SimpleFeatureComparator::SimpleFeatureComparator(Feature* feature_in) {
		feature = feature_in;
		initialized = false;
	}

	SimpleFeatureComparator::~SimpleFeatureComparator() {
	}
	
	/*---------------------*
	 * General attributes. *
	 *---------------------*/
	
	Feature* SimpleFeatureComparator::getFeature() {
		return feature;
	}
	
	/*---------------------*
	 * Gaussian attributes. *
	 *---------------------*/
	
	double SimpleFeatureComparator::getMean() {
		return featureMean;
	}
	
	double SimpleFeatureComparator::getVariance() {
		return featureVariance;
	}
	
	/*---------------------*
	 * Fit the Gaussian     *
	 *---------------------*/
	void SimpleFeatureComparator::fitGaussian(){
		int history_size = feature->getHistorySize();
		
		//calculate the mean
		featureMean=0;
		for (int i =0; i < history_size; i++)
			featureMean += feature->getHistoryFrame(i);
		featureMean /= double(history_size);
		
		//calculate the variance (unbiased formulation)
		featureVariance=0;
		double meanCenter;
		for (int i =0; i < history_size; i++){
			meanCenter = feature->getHistoryFrame(i) - featureMean;
			featureVariance += (meanCenter*meanCenter);
		}
		featureVariance /= double(history_size-1);
		if (featureVariance == 0)
			featureVariance = numeric_limits<double>::min(); //just in case to avoid division by zero
		
	}
	

	
	/*-------------*
	 * Operations. *
	 *-------------*/

	void SimpleFeatureComparator::initialize() {
		fitGaussian();
		initialized = true;
	}
	
	bool SimpleFeatureComparator::isInitialized() {
		return initialized;
	}
	
	double SimpleFeatureComparator::compare(SimpleFeatureComparator* model) {
		if (!initialized)
			initialize();
		
		if (!model->isInitialized())
			model->initialize();
			
		Feature* other_trajectory = model->getFeature();
		int frames = other_trajectory->getHistorySize();
		double meanCenter, log_lik = 0;
		
		for (int i = 0; i < frames; i++){
			meanCenter = other_trajectory->getHistoryFrame(i) - featureMean;
			log_lik -= (0.5 * meanCenter * meanCenter / featureVariance);
		}
		log_lik -= frames * log(2 * PI * featureVariance);
		return log_lik / double(frames);
		
	}
}
