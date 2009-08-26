/*
	Copyright 2007, 2008, 2009 Brandon Mechtley, Gordon Wichern, and Harvey Thornburg

	This file is part of Sirens.

	Sirens is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Sirens is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Sirens.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Segmenter.h"

#include <cmath>
using namespace std;

namespace Sirens {
	Segmenter::Segmenter(double p_new, double p_off) {
		setPNew(p_new);
		setPOff(p_off);
		
		initialized = false;
	}
	
	Segmenter::~Segmenter() {		
	}
	
	void Segmenter::setPNew(double value) {
		pNew = value;
	}
	
	void Segmenter::setPOff(double value) {
		pOff = value;
	}
	
	double Segmenter::getPNew() {
		return pNew;
	}
	
	double Segmenter::getPOff() {
		return pOff;
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
	
	void Segmenter::viterbi(int frame) {
		int edges = getEdges();
		
		for (int new_index = 0; new_index < edges; new_index++) {
			for (int old_index = 0; old_index < edges; old_index++) {
				double cost_temp = 0;
				
				for (int feature_index = 0; feature_index < features.size(); feature_index++) {
					SegmentationParameters* parameters = features[feature_index]->getSegmentationParameters();
					
					newDistributions[feature_index][new_index][old_index].cost = KalmanLPF(
						y[feature_index],
						newDistributions[feature_index][new_index][old_index].covariance,
						newDistributions[feature_index][new_index][old_index].mean,
						parameters->getR(),
						parameters->q[modeMatrix[feature_index + 1][old_index] - 1][modeMatrix[feature_index + 1][new_index] - 1],
						parameters->getAlpha()
					);
					
					cost_temp += newDistributions[feature_index][new_index][old_index].cost;
				}
				
				costs[new_index][old_index] = oldCosts[old_index] + cost_temp - probabilityMatrix[new_index][old_index];
			}
		}
		
		for (int i = 0; i < costs.size(); i++) {
			vector<double>::iterator minimum_iterator = min_element(costs[i].begin(), costs[i].end());
			psi[frame][i] = distance(costs[i].begin(), minimum_iterator);
			oldCosts[i] = *minimum_iterator;
		}
		
		// Copy best filtered states and covariances.
		for (int i = 0; i < edges; i++) {
			for (int feature_index = 0; feature_index < features.size(); feature_index++) {
				for (int row = 0; row < 2; row++) {
					maxDistributions[feature_index][i].mean[row] = newDistributions[feature_index][i][psi[frame][i]].mean[row];
					
					for (int column = 0; column < 2; column++)
						maxDistributions[feature_index][i].covariance[row][column] = newDistributions[feature_index][i][psi[frame][i]].covariance[row][column];
				}
			}
		}
	}
	
	void Segmenter::initialize() {
		if (!initialized) {
			for (int i = 0; i < features.size(); i++)
				features[i]->getSegmentationParameters()->initialize();
			
			createModeLogic();
			createProbabilityTable();
			
			int edges = getEdges();
			modes = vector<int>(frames, 0);
			
			vector<double> cost_vector = vector<double>(edges, 0);
			costs = vector<vector<double> >(edges, cost_vector);
			oldCosts = vector<double>(edges, 0);
			
			vector<int> psi_row = vector<int>(edges, 0);
			psi = vector<vector<int> >(frames, psi_row);
			
			vector<ViterbiDistribution> temp1(edges);
			maxDistributions = vector<vector<ViterbiDistribution> >(features.size(), temp1);
			
			vector<vector<ViterbiDistribution> > temp2(edges, temp1);
			newDistributions = vector<vector<vector<ViterbiDistribution> > >(features.size(), temp2);
			
			for (int i = 0; i < features.size(); i++) {
				for (int a = 0; a < 2; a++) {
					maxDistributions[i][0].mean[a] = features[i]->getSegmentationParameters()->xInit[a];
					
					for (int b = 0; b < 2; b++)
						maxDistributions[i][0].covariance[a][b] = features[i]->getSegmentationParameters()->pInit[a][b];
				}
			}
			
			y = vector<double>(features.size(), 0);
			
			initialized = true;
		}
	}
	
	void Segmenter::segment(FeatureSet* feature_set) {
		features = feature_set->getFeatures();
		frames = feature_set->getMinHistorySize();
		
		initialize();
				
		for (int i = 0; i < frames; i++) {
			for (int j = 0; j < features.size(); j++)
				y[j] = features[j]->getHistoryFrame(i);
			
			viterbi(i);
		}
		
		vector<int> state_sequence(frames, 0);
		vector<double>::iterator minimum_cost = min_element(oldCosts.begin(), oldCosts.end());
		state_sequence[frames - 1] = distance(oldCosts.begin(), minimum_cost);
		
		for (int i = frames - 2; i > -1; i--) 
			state_sequence[i] = psi[i][state_sequence[i + 1]];
		
		for (int i = 0; i < frames; i++)
			modes[i] = modeMatrix[0][state_sequence[i]];
	}
	
	vector<vector<double> > Segmenter::getSegments() {
		vector<vector<double> > segments;
		vector<int> starts;
		vector<int> stops;
		
		if (modes.size() > 1 && (modes[modes.size() - 1] == 2))
			modes[modes.size() - 1] = modes[modes.size() - 2];
		
		for (int i = 0; i < modes.size(); i++) {	
			if ((i == 0 && modes[i] == 3) || (modes[i] == 2))
				starts.push_back(i);
			else if ((i > 0) && (modes[i - 1] == 3) && (modes[i] != 3))
				stops.push_back(i);
		}
		
		for (int i = 0; i < starts.size(); i++) {
			vector<double> segment(2);
			segment[0] = starts[i];
			segment[1] = (i >= stops.size()) ? frames - 1 : stops[i];
			
			segments.push_back(segment);
		}
		
		return segments;
	}
	
	vector<int> Segmenter::getModes() {
		return modes;
	}
}
