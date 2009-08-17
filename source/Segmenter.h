#ifndef __SEGMENTER_H__
#define __SEGMENTER_H__

#include "Feature.h"
#include "FeatureSet.h"

#include <vector>
using namespace std;

namespace Sirens {
	struct Beam {
		int oldMode;
		int newMode;
		int oldSequenceNumber;
		double cost;
	};
	
	// Dynamic-state bayes network.
	class DSBN {
	public:
		double mean[2];
		double covariance[2][2];
		double cost;
		
		DSBN() {
			mean[0] = mean[1] = 0.0;
			covariance[0][0] = covariance[0][1] = covariance[1][0] = covariance[1][1] = 0.0;
			cost = 0.0;
		}
	};
	
	class Segmenter {
	private:
		bool initialized;
		
		// Initialization.
		int beams, tempBeams;
		
		double pNew, pOff;
		
		vector<double> y;
		
		vector<vector<double> > modeTransitions;
		vector<vector<double> > probabilityMatrix;
		vector<vector<int> > modeMatrix;
		
		vector<Feature*> features;
		int featureCount;
		
		// Runtime.
		vector<Beam*> oldBeams, newBeams;
		vector<vector<int> > oldPsi, newPsi;
		int oldMode, newMode;
		vector<vector<DSBN> > oldDSBN, newDSBN;
		vector<vector<vector<DSBN> > > tempDSBN;
		
		bool start, stop;
		
		// Methods.
		vector<int> getFeatureIndices(int feature);
		int getEdges();
		double KalmanLPF(double y, double p[2][2], double x[2], double r, double q, double alpha);
		
	public:	
		Segmenter(double p_new = 0, double p_old = 0, int nbeams = 20);
		~Segmenter();
		
		// Attributes.
		void setPNew(double value);
		void setPOff(double value);
		void setBeams(int value);
		
		double getPNew();
		double getPOff();
		int getBeams();
		
		// Operations.
		void createModeLogic();
		void createProbabilityTable();
		void beamSearch();
		void initialize();
		
		vector<vector<double> > getSegments(FeatureSet* features);
	};
	
	static bool compareCost(Beam* Bm1, Beam* Bm2) {
		return Bm1->cost < Bm2->cost;
	}
}

#endif
