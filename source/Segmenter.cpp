#include "Segmenter.h"

#include <iostream>
#include <cmath>
using namespace std;

namespace Sirens {
	Segmenter::Segmenter(double p_new, double p_off, int nbeams) {
		setPNew(p_new);
		setPOff(p_off);
		setBeams(nbeams);
		
		initialized = false;
	}
	
	Segmenter::~Segmenter() {
		for (int i = 0; i < oldBeams.size(); i++)
			delete oldBeams[i];
		
		for (int i = 0; i < newBeams.size(); i++)
			delete newBeams[i];
	}
	
	void Segmenter::setPNew(double value) {
		pNew = value;
	}
	
	void Segmenter::setPOff(double value) {
		pOff = value;
	}
	
	void Segmenter::setBeams(int value) {
		beams = value;
	}
	
	double Segmenter::getPNew() {
		return pNew;
	}
	
	double Segmenter::getPOff() {
		return pOff;
	}
	
	int Segmenter::getBeams() {
		return beams;
	}
	
	void Segmenter::createModeLogic() {
		vector<double> row(3, 0);
		modeTransitions = vector<vector<double> >(3, row);
		
		modeTransitions[0][0] = 1.0 - pNew;
		modeTransitions[0][1] = pNew;
		modeTransitions[0][2] = 0;
		
		modeTransitions[1][0] = 0;
		modeTransitions[1][1] = 0;
		modeTransitions[1][2] = 1;
		
		modeTransitions[2][0] = pOff - pOff * pNew;
		modeTransitions[2][1] = pNew;
		modeTransitions[2][2] = 1.0 - pNew - pOff + pOff * pNew;
	}
	
	vector<int> Segmenter::getFeatureIndices(int feature) {
		vector<int> indices(features.size() + 1, 0);
		
		indices[0] = int(ceil(double(feature) / double(getEdges() / 3)));
		
		for (int i = 0; i < features.size(); i++) {			
			int top_index = pow(3.0, double(features.size() - i));
			int bottom_index = pow(3.0, double(features.size() - i - 1));
						
			indices[i + 1] = ceil(double(feature % top_index) / double(bottom_index));
						
			if (indices[i + 1] == 0)
				indices[i + 1] = 3;
		}
	
		return indices;
	}
	
	void Segmenter::createProbabilityTable() {		
		int mode_old, mode_new, feature_mode_old, feature_mode_new;
		double gate_probability;
		int edges = getEdges();
		
		vector<double> double_row(edges);
		probabilityMatrix = vector<vector<double> >(edges, double_row);
		
		vector<int> int_row(edges);
		modeMatrix = vector<vector<int> >(int(features.size() + 1), int_row);
				
		for (int i = 0; i < edges; i++) {
			vector<int> indices = getFeatureIndices(i + 1);
			
			for (int j = 0; j < features.size() + 1; j++)
				modeMatrix[j][i] = indices[j];
		}
		
		for (int i = 0; i < edges; i++) {
			for (int j = 0; j < edges; j++) {
				mode_old = modeMatrix[0][i];
				mode_new = modeMatrix[0][j];
				gate_probability = 1.0;
				
				for (int k = 0; k < features.size(); k++) {
					feature_mode_old = modeMatrix[k + 1][i];
					feature_mode_new = modeMatrix[k + 1][j];
					
					gate_probability *= features[k]->getSegmentationParameters()->fusionLogic[mode_old - 1][mode_new - 1][feature_mode_old - 1][feature_mode_new - 1];
				}
					
				probabilityMatrix[j][i] = modeTransitions[mode_old - 1][mode_new - 1] * gate_probability;
				
				if (probabilityMatrix[j][i] == 0)
					probabilityMatrix[j][i] = -46.0517;
				else
					probabilityMatrix[j][i] = log(probabilityMatrix[j][i]);
			}
		} 
	}
	
	int Segmenter::getEdges() {
		return 3 * pow(3.0, double(features.size()));
	}
	
	double Segmenter::KalmanLPF(double y, double p[2][2], double x[2], double r, double q, double alpha) {
		double k[2];
		double err;
		double s;
		
		x[1] = (1 - alpha) * x[0] + alpha * x[1];
		p[1][1] = p[0][0] * (1 - alpha) * (1 - alpha) + 2 * p[0][1] * alpha * (1 - alpha) + p[1][1] * alpha * alpha + q * (1 - alpha) * (1 - alpha);
		p[1][0] = p[0][0] * (1 - alpha) + p[1][0] * alpha + q * (1 - alpha);
		p[0][1] = p[1][0];
		p[0][0] = p[0][0] + q;
		
		err = y - x[1];
		s = p[1][1] + r;
		
		k[0] = p[0][1] / s;
		k[1] = p[1][1] / s;
		
		p[0][0] -= k[0] * p[0][1];
		p[1][0] -= k[0] * p[1][1];
		p[0][1] = p[1][0];
		p[1][1] -= k[1] * p[1][1];
		
		x[0] += k[0] * err;
		x[1] += k[1] * err;
		
		return 0.5 * (log(s) + (err * err / s));
	}
	
	void Segmenter::beamSearch() {
		int stp, old_ind, tmp;
		int edges = getEdges();
		stp = getEdges() / 3;
		
		for (int j = 0; j < features.size(); j++) {
			for (int i = 0; i < 3; i++) {
				for (int k = 0; k < tempBeams; k++) {
					old_ind = modeMatrix[j + 1][newBeams[k]->oldMode] - 1;
					
					tempDSBN[j][i][k] = oldDSBN[j][k];
					
					SegmentationParameters* parameters = features[j]->getSegmentationParameters();
					
					tempDSBN[j][i][k].cost = KalmanLPF(
							y[j], 
							tempDSBN[j][i][k].covariance, 
							tempDSBN[j][i][k].mean, 
							parameters->getR(), 
							parameters->q[old_ind][i],
							parameters->getAlpha());
				}
			}
		}
		
		int k[features.size()];
		
		for (int i = 0; i < stp; i++) {
			for (int j = features.size() - 1; j > -1; j--)
				k[j] = (i / int(pow(3.0, double(features.size() - j - 1)))) % 3;
			
			for (int j = 0; j < tempBeams; j++) {
				Beam* new_beam_1 = newBeams[i + edges * j];
				Beam* new_beam_2 = newBeams[i + stp + edges * j];
				Beam* new_beam_3 = newBeams[i + stp * 2 + edges * j];
				Beam* old_beam = oldBeams[j];
				
				new_beam_1->cost = old_beam->cost - probabilityMatrix[i][old_beam->oldMode];
				new_beam_2->cost = old_beam->cost - probabilityMatrix[i + stp][old_beam->oldMode];
				new_beam_3->cost = old_beam->cost - probabilityMatrix[i + stp * 2][old_beam->oldMode];
				
				for (int l = 0; l < features.size(); l++) {
					new_beam_1->cost += tempDSBN[l][k[l]][j].cost;
					new_beam_2->cost += tempDSBN[l][k[l]][j].cost;
					new_beam_3->cost += tempDSBN[l][k[l]][j].cost;
				}
				
				new_beam_1->oldSequenceNumber = j;
				new_beam_2->oldSequenceNumber = j;
				new_beam_3->oldSequenceNumber = j;

				new_beam_1->oldMode = i;
				new_beam_2->oldMode = i + stp;
				new_beam_3->oldMode = i + stp * 2;
			}
		}
		
		partial_sort(newBeams.begin(), newBeams.begin() + tempBeams, newBeams.end(), compareCost);
		
		if (tempBeams < beams) {
			tmp = 0;
			
			for (int i = 0; i < tempBeams; i++)
				tmp = tmp + edges;
			
			if (tmp < beams)
				tempBeams = tmp;
			else
				tempBeams = beams;
		}
		
		for (int i = 0; i < tempBeams; i++) {
			for (int j = 0; j < features.size(); j++) {
				int tempp = modeMatrix[j + 1][newBeams[i]->oldMode] - 1;
				oldDSBN[j][i] = tempDSBN[j][tempp][newBeams[i]->oldSequenceNumber];
			}
			
			newPsi[i] = oldPsi[newBeams[i]->oldSequenceNumber];
			
			if (newPsi[i].size() > 50)
				newPsi[i].erase(newPsi[i].begin());
			
			newPsi[i].push_back(newBeams[i]->oldMode);
		}
		
		if (newPsi[0].size() > 50)
			newMode = modeMatrix[0][newPsi[0][0]];
		else
			newMode = 1;
		
		start = (newMode == 2);
		stop = ((oldMode == 3) && (newMode != 3));
		
		oldMode = newMode;
		oldPsi = newPsi;
		vector<Beam*> temp_beams = oldBeams;
		oldBeams = newBeams;
		newBeams = temp_beams;
	}
	
	void Segmenter::initialize() {
		if (!initialized) {
			for (int i = 0; i < features.size(); i++)
				features[i]->getSegmentationParameters()->initialize();
			
			createModeLogic();
			createProbabilityTable();
			
			int edges = getEdges();
			
			tempBeams = 1;
			
			oldBeams = vector<Beam*>(edges * beams);
			for (int i = 0; i < oldBeams.size(); i++)
				oldBeams[i] = new Beam();
				
			newBeams = vector<Beam*>(edges * beams);
			for (int i = 0; i < newBeams.size(); i++)
				newBeams[i] = new Beam();
			
			vector<DSBN> temp1(beams);
			oldDSBN = vector<vector<DSBN> >(features.size(), temp1);
			
			vector<vector<DSBN> > temp2(3, temp1);
			tempDSBN = vector<vector<vector<DSBN> > >(features.size(), temp2);
			
			for (int i = 0; i < features.size(); i++) {
				for (int a = 0; a < 2; a++) {
					oldDSBN[i][0].mean[a] = features[i]->getSegmentationParameters()->xInit[a];
					
					for (int b = 0; b < 2; b++)
						oldDSBN[i][0].covariance[a][b] = features[i]->getSegmentationParameters()->pInit[a][b];
				}
			}
			
			newPsi = vector<vector<int> >(beams);
			oldPsi = vector<vector<int> >(beams);
			
			y = vector<double>(features.size(), 0);
			
			start = false;
			stop = false;
			
			initialized = true;
		}
	}
	
	vector<vector<double> > Segmenter::getSegments(FeatureSet* feature_set) {
		features = feature_set->getFeatures();
		
		vector<double> starts;
		vector<double> stops;
		vector<vector<double> > segments;
		
		initialize();
		
		// true: start, false: stop
		bool last_signal = true;
		starts.push_back(0);
		
		for (int i = 0; i < feature_set->getMinHistorySize(); i++) {
			if ((i % 1000) == 0)
				cout << "Frame " << i << endl;
				
			for (int j = 0; j < features.size(); j++)
				y[j] = features[j]->getHistoryFrame(i);
			
			beamSearch();
						
			if (start) {
				if (last_signal)
					stops.push_back(i);
				
				starts.push_back(i);
				
				last_signal = true;
			}
			
			if (stop) {
				if (last_signal)
					stops.push_back(i);
				
				last_signal = false;
			}
		}
		
		// If there is a final, unstopped segmented, implicitly stop it at the end of the sound.
		if (starts.size() > stops.size())
			stops.push_back(feature_set->getMinHistorySize() - 1);
		
		// Segments are between starts and stops.
		while (starts.size() > 0 && stops.size() > 0) {
			vector<double> segment(2);
			segment[0] = starts.front();
			segment[1] = stops.front();
			
			starts.erase(starts.begin());
			stops.erase(stops.begin());
			
			segments.push_back(segment);
		}
		
		return segments;
	}
}
