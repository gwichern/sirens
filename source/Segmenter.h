#ifndef __SEGMENTER_H__
#define __SEGMENTER_H__

#include "Feature.h"
#include "FeatureSet.h"

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
