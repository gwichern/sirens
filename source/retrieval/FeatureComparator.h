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

#ifndef __FEATURECOMPARATOR_H__
#define __FEATURECOMPARATOR_H__

#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/matrix.hpp"
using namespace boost::numeric;

#include "../Feature.h"

namespace Sirens {
	// Parameters for least-squares curve fitting.
	struct LeastSquaresParameters {
		int order;									// 0 = constant, 1 = linear, 2 = quadratic.
		
		ublas::matrix<double> covariance;
		ublas::matrix<double> covarianceInverse;
		double covarianceDeterminant;
		
		ublas::vector<double> trajectory;			// Smoothed trajectory from fit.
		
		ublas::vector<double> coefficients;			// Curve polynomial coefficients.
		double aicc;								// Akaike information criterion.
		
		double xOffset;								// mean of the observations.
		
		ublas::matrix<double> mean;
		ublas::vector<double> timeSamples;
	};
	
	// Class that creates a retrieval model for an individual feature trajectory.
	class FeatureComparator {
	private:
		Feature* feature;							// feature trajectory that this model represents.
		bool initialized;
		
		// Savitzky-Golay smoothing.
		ublas::matrix<double> trajectory;			// row 0 is position; row 1 is velocity.
		
		// Curve fitting.
		LeastSquaresParameters* bestFit;
		
		// HMM.
		ublas::vector<double> prior;
		ublas::matrix<double> transitions;
		
		// Helpers.
		void fitPolynomial(LeastSquaresParameters* curve, ublas::vector<double>* concatonated_trajectory);
		double f0(double distance);
		double f1(double distance1, double distance2);
		double f2(double distance2, double distance3);
		double forwardAlgorithm(ublas::matrix<double> emission_probabilities);
		
		// Initialization.
		void smooth();								// 1. Perform Savitzky-Golay smoothing of the feature trajectory.
		void fitCurve();							// 2. Find the least-squares best-fitting curve (constant, linear, quadratic).
		void createHMM();							// 3. Create the HMM that represents the query distribution for the sound.
		
	public:
		FeatureComparator(Feature* feature_in);
		~FeatureComparator();
		
		// General attributes.
		Feature* getFeature();
		bool isInitialized();
		
		// Savitzky-Golay trajectories.
		ublas::matrix<double> getTrajectory();
		
		// Curve fitting attributes.
		ublas::matrix<double> getCovariance();
		ublas::matrix<double> getCovarianceInverse();
		double getCovarianceDeterminant();
		
		// HMM attributes.
		ublas::vector<double> getPrior();
		ublas::matrix<double> getTransitions();
		
		// Operations.
		void initialize();
		double compare(FeatureComparator* model);
	};
}

#endif
