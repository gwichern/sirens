/*
	Copyright 2007, 2008, 2009 Brandon Mechtley, Gordon Wichern, and Harvey Thornburg
	
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

#ifndef __SEGMENTER_H__
#define __SEGMENTER_H__

#include "../Feature.h"
#include "../FeatureSet.h"

#include <vector>
using namespace std;

namespace Sirens {
	class ViterbiDistribution {
	public:
		double mean[2];
		double covariance[2][2];
		double cost;
		
		ViterbiDistribution() {
			mean[0] = mean[1] = 0.0;
			covariance[0][0] = covariance[0][1] = covariance[1][0] = covariance[1][1] = 0.0;
			cost = 0.0;
		}
	};
	
	class Segmenter {
	private:
		bool initialized;													// on-demand initialization (called from segment())
		
		// Initialization.
		int frames;
		
		double pNew, pOff;													// global mode parameters
		
		vector<double> y;													// feature vector
		
		vector<vector<double> > modeTransitions;							// mode transition probabilities
		vector<vector<double> > probabilityMatrix;							// global probability matrix (log-scaled)
		
		vector<vector<int> > modeMatrix;									// global modes
				
		vector<vector<double> > costs;										// cost matrix for viterbi algorithm
		vector<vector<int> > psi;											// stored state sequences
		vector<double> oldCosts;											// cost list for previous frame
		
		vector<Feature*> features;											// features used for segmentation (get parameters and trajectories here)
		
		// Runtime.
		vector<vector<ViterbiDistribution> > maxDistributions;				// most likely gaussian distributions of state vectors for each feature
		vector<vector<vector<ViterbiDistribution> > > newDistributions;		// gaussian distribution of state vectors for each feature
		
		// Methods.
		vector<int> getFeatureIndices(int feature);							// return indices of the global mode matrix for the given feature.
		int getEdges();														// 3 ^ (features + 1)
		
		// Kalman Filter for smoothing input feature trajectories.
		double KalmanLPF(double y, double p[2][2], double x[2], double r, double q, double alpha);
		
		vector<int> modes;
		
	public:	
		Segmenter(double p_new = 0, double p_old = 0);
		~Segmenter();
		
		// Attributes.
		void setPNew(double value);
		void setPOff(double value);
		void setDelay(int value);
		
		double getPNew();
		double getPOff();
		int getDelay();
		
		// Initialization.
		void createModeLogic();
		void createProbabilityTable();
		void initialize();				
		
		// Segmentation. TODO: See about replacing this with a working beam search.
		void viterbi(int frame);											// viterbi algorithm.		
		void segment(FeatureSet* features);									// perform segmentation, create global mode trajectory
		
		// After segmentation.
		vector<vector<double> > getSegments();								// get activated segments
		vector<int> getModes();												// get global mode trajectory
	};
}

#endif
