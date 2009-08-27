/*
	Copyright 2009 Brandon Mechtley, Gordon Wichern, and Jiachen Xue
	
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

#include "RetrievalModel.h"

#include "support/matrix_support.h"
#include "support/math_support.h"

#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <iostream>
using namespace std;

namespace Sirens {
	RetrievalModel::RetrievalModel(Feature* feature_in) {
		feature = feature_in;
		
		bestFit = NULL;
		
		trajectory = new ublas::matrix<double>();
		
		prior = new ublas::vector<double>();
		transitions = new ublas::matrix<double>();
	}

	RetrievalModel::~RetrievalModel() {
		delete trajectory;

		if (bestFit != NULL)
			delete bestFit;
			
		delete prior;
		delete transitions;
	}

	/*----------*
	 * Helpers. *
	 *----------*/
	
	void RetrievalModel::fitPolynomial(LeastSquaresParameters* curve, ublas::vector<double>* concatonated_trajectory) {
		int size = trajectory->size2();
		
		ublas::matrix<double> x = ublas::zero_matrix<double>(2 * size, curve->order + 1);
		
		for (int i = 0; i < size; i++) {
			if (curve->order > 1) {
				x(i, 0) = (i + 1) * (i + 1);
				x(size + i, 0) = 2 * (i + 1);
			}
			
			if (curve->order > 0) {
				x(i, curve->order - 1) = (i + 1);
				x(size + i, curve->order - 1) = 1;
			}
			
			x(i, curve->order) = 1;
			x(size + i, curve->order) = 0;
		}
		
		curve->coefficients = ublas::prod(ublas::prod(invert(ublas::prod(ublas::trans(x), x)), ublas::trans(x)), (*concatonated_trajectory));
		
		curve->trajectory = ublas::prod(x, curve->coefficients);
		
		ublas::vector<double> error = (*concatonated_trajectory) - curve->trajectory;
		
		double rss = 0;
		
		for (int i = 0; i < error.size(); i++)
			rss += error[i] * error[i];
		
		ublas::matrix<double> error_matrix = ublas::zero_matrix<double>(2, size);
		for (int i = 0; i < size; i++) {
			error_matrix(0, i) = error[i];
			error_matrix(1, i) = error[size + i];
		}
		;
		// Calculate mean error for position and velocity trajectories.
		ublas::vector<double> mean = ublas::zero_vector<double>(2);
		
		for (int i = 0; i < size; i++)
			mean += ublas::matrix_column<ublas::matrix<double> >(error_matrix, i);
		
		mean /= size;
		
		ublas::vector<double> ones(size);
		
		for (int i = 0; i < ones.size(); i++)
			ones[i] = 1;
		
		ublas::matrix<double> value = ublas::outer_prod(mean, ones);
		
		error_matrix = error_matrix - value;
		
		curve->covariance = (1.0 / double(size - 1)) * ublas::prod(error_matrix, ublas::trans(error_matrix));
		
		curve->aicc = akaike(curve->order + 1, 2 * size, rss);
	}
	
	/*-----------------*
	 * Initialization. *
	 *-----------------*/
	
	// Savitzky-Golay smoothing of position and velocity vectors for feature trajectory.
	void RetrievalModel::smooth() {
		// Iteration variables.
		int padding = (savitzky_golay_f + 1) / 2;
		int history_size = feature->getHistorySize();
	
		// Fill two vectors with the first 2 differentiation filters of a 4th-order SG filter for 41 frames.
		ublas::vector<double> first;
		ublas::vector<double> second;
		fill_savitzky_golay(first, second);
		
		// Resize position and velocity vectors for filling.
		trajectory->resize(2, history_size);
		
		// Fill the trajectory vector with history values, padded with 1s at the ends.
		ublas::vector<double> padded_position(history_size + padding * 2);
		for (int i = 0; i < padding; i++)
			padded_position[i] = feature->getHistoryFrame(0);
	
		for (int i = history_size + padding - 1; i < history_size + padding * 2; i++)
			padded_position[i] = feature->getHistoryFrame(history_size - 1);
	
		for (int i = padding; i < history_size + padding - 1; i++)
			padded_position[i] = feature->getHistoryFrame(i - padding);
		
		// Calculate smoothed position and velocity by sliding the SG filters across the trajectory.
		for (int i = padding - 1; i < history_size + padding - 1; i++) {
			int index = i - (padding - 1);
			
			ublas::vector<double> window(padding * 2 - 1);
			
			for (int j = 0; j < padding * 2 - 1; j++)
				window[j] = padded_position[i - padding + 1 + j];
			
			(*trajectory)(0, index) = ublas::inner_prod(first, window);
			(*trajectory)(1, index) = ublas::inner_prod(second, window);
		}
	}
	
	void RetrievalModel::fitCurve() {
		int size = trajectory->size2();
		vector<LeastSquaresParameters*> curves(3);
		ublas::vector<double>* concatonated_trajectory = new ublas::vector<double>(size * 2);
		
		for (int i = 0; i < size; i++) {
			(*concatonated_trajectory)[i] = (*trajectory)(0, i);
			(*concatonated_trajectory)[i + size] = (*trajectory)(1, i);
		}
		
		for (int i = 0; i < 3; i++) {
			curves[i] = new LeastSquaresParameters();
			curves[i]->order = i;
			
			fitPolynomial(curves[i], concatonated_trajectory);
			
			if (bestFit == NULL || bestFit->aicc > curves[i]->aicc)
				bestFit = curves[i];
		}
		
		for (int i = 0; i < 2; i++) {
			if (bestFit != curves[i])
				delete curves[i];
		}
		
		bestFit->mean = ublas::zero_matrix<double>(2, 2 * bestFit->order + 1);
		
		ublas::vector<double> positions = ublas::vector_range<ublas::vector<double> >(bestFit->trajectory, ublas::range(0, size));
		
		bestFit->xOffset = 0;
		
		for (int i = 0; i < size; i++)
			bestFit->xOffset += positions[i];
		
		bestFit->xOffset /= size;
		
		if (bestFit->order == 0) {
			bestFit->mean(0, 0) = bestFit->coefficients(0);				// Position state is constant.
			bestFit->mean(1, 0) = 0;									// Velocity state is 0.
			
			bestFit->timeSamples = ublas::zero_vector<double>(0);	
		} else if (bestFit->order == 1) {
			double midpoint = round(double(size - 1) / 2.0) - 1;
			
			// Position states.
			bestFit->mean(0, 0) = positions[0];
			bestFit->mean(0, 1) = positions[midpoint];
			bestFit->mean(0, 2) = positions[size - 1];
			
			// Velocity states are constant.
			bestFit->mean(1, 0) = bestFit->coefficients(0);
			bestFit->mean(1, 1) = bestFit->coefficients(0);
			bestFit->mean(1, 2) = bestFit->coefficients(0);
			
			bestFit->timeSamples.resize(2);
			bestFit->timeSamples[0] = midpoint;
			bestFit->timeSamples[1] = size - 1 - midpoint;
		} else if (bestFit->order == 2) { 
			int vertex = round(-bestFit->coefficients(1) / (2.0 * bestFit->coefficients(0))) - 1;	// vertex of parabola.
			if (vertex < 0 || vertex >= size)
				vertex = round(double(size) / 2.0) - 1;
			
			int midpoint1 = round(double(vertex) / 2.0);
			int midpoint2 = round(double(size - 1 - vertex) / 2.0) + vertex;
			
			// Position states.
			bestFit->mean(0, 0) = positions[0];
			bestFit->mean(0, 1) = positions[midpoint1];
			bestFit->mean(0, 2) = positions[vertex];
			bestFit->mean(0, 3) = positions[midpoint2];
			bestFit->mean(0, 4) = positions[size - 1];
			
			// Velocity states.
			bestFit->mean(1, 0) = 2 * bestFit->coefficients[0] + bestFit->coefficients[1];
			bestFit->mean(1, 1) = 2 * bestFit->coefficients[0] * midpoint1 + bestFit->coefficients[1];
			bestFit->mean(1, 2) = 2 * bestFit->coefficients[0] * vertex + bestFit->coefficients[1];
			bestFit->mean(1, 3) = 2 * bestFit->coefficients[0] * midpoint2 + bestFit->coefficients[1];
			bestFit->mean(1, 4) = 2 * bestFit->coefficients[0] * (size - 1) + bestFit->coefficients[1];
			
			bestFit->timeSamples.resize(4);
			bestFit->timeSamples[0] = midpoint1 - 1;
			bestFit->timeSamples[1] = vertex - midpoint1 - 1;
			bestFit->timeSamples[2] = midpoint2 - vertex - 1;
			bestFit->timeSamples[3] = size - 1 - midpoint2 - 1;
		}
	}
		
	void RetrievalModel::createHMM() {
		// Create the prior state.
		prior = ublas::matrix_row<ublas::matrix<double> >(ublas::identity_matrix<double>(2 * bestFit->order + 1), 0);
		
		// Create the transition probability matrix between states.
		if (bestFit->order == 0)
			transitions = scalar_matrix<double>(1, 1, 1.0);
		else if (bestFit->order == 1) {
			transitions = ublas::zero_matrix<double>(3, 3);
			
			transitions(0, 0) = f0(bestFit->timeSamples[0]);
			transitions(0, 1) = f1(bestFit->timeSamples[0], bestFit->timeSamples[1]);
			transitions(0, 2) = f2(bestFit->timeSamples[0], bestFit->timeSamples[1]);
			transitions(1, 1) = f0(bestFit->timeSamples[1]);
			transitions(1, 2) = 1 - transitions(1, 1);
			transitions(2, 2) = 1;
		} else if (bestFit->order == 2) {
			transitions = ublas::zero_matrix<double>(5, 5);
			
			transitions(0, 0) = f0(bestFit->timeSamples[0]);
			transitions(0, 1) = f1(bestFit->timeSamples[0], bestFit->timeSamples[1]);
			transitions(0, 2) = f2(bestFit->timeSamples[0], bestFit->timeSamples[1]);
			transitions(1, 1) = f0(bestFit->timeSamples[1]);
			transitions(1, 2) = f1(bestFit->timeSamples[1], bestFit->timeSamples[2]);
			transitions(1, 3) = f2(bestFit->timeSamples[1], bestFit->timeSamples[2]);
			transitions(2, 2) = f0(bestFit->timeSamples[2], bestFit->timeSamples[3]);
			transitions(2, 3) = f1(bestFit->timeSamples[2], bestFit->timeSamples[3]);
			transitions(2, 4) = f2(bestFit->timeSamples[2], bestFit->timeSamples[3]);
			transitions(3, 3) = f0(bestFit->timeSamples[3]);
			transitions(3, 4) = 1 - transitions(3, 3);
			transitions(4, 4) = 1;
		}
		
		/*
	    det_cov_mat = det(cov_mat);
	    cov_mat_inverse = (cov_mat)^(-1);

	    eMat = zeros(num_states,num_frames);
	    for ii = 1:num_states
	        for jj = 1:num_frames
	            eMat(ii,jj) = (2*pi*(det_cov_mat)^(0.5))^(-1)*exp(-0.5*(feature_data(:,jj) - mean_mat(:,ii))'*cov_mat_inverse*(feature_data(:,jj) - mean_mat(:,ii)));
	        end;
	    end;

	    %calculate the probability using the forward algorithm
	    for_warning = 0;
	    [feature_log_pr(j),for_warning] = ForwardAlg (PI,pMat,eMat);
	 	*/
	}

	/*------------------------------*
	 * Savitzky-Golay trajectories. *
	 *------------------------------*/

	ublas::matrix<double>* RetrievalModel::getTrajectory() {
		return trajectory;
	}

	/*-----------------*
	 * HMM attributes. *
	 *-----------------*/

	ublas::vector<double>* RetrievalModel::getPrior() {
		return prior;
	}

	ublas::matrix<double>* RetrievalModel::getTransitions() {
		return transitions;
	}

	/*-------------*
	 * Operations. *
	 *-------------*/

	void RetrievalModel::initialize() {
		smooth();
		fitCurve();
		createHMM();
	}
}