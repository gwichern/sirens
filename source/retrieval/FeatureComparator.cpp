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

#include "FeatureComparator.h"

#include "../support/matrix_support.h"
#include "../support/math_support.h"

#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <iostream>
using namespace std;

namespace Sirens {
	FeatureComparator::FeatureComparator(Feature* feature_in) {
		feature = feature_in;
		
		bestFit = NULL;
	}

	FeatureComparator::~FeatureComparator() {
		if (bestFit != NULL)
			delete bestFit;
	}
	
	/*---------------------*
	 * General attributes. *
	 *---------------------*/
	
	Feature* FeatureComparator::getFeature() {
		return feature;
	}
	
	/*----------*
	 * Helpers. *
	 *----------*/
	
	void FeatureComparator::fitPolynomial(LeastSquaresParameters* curve, ublas::vector<double>* concatonated_trajectory) {
		int size = trajectory.size2();
		
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
		curve->covariance = (1.0 / double(size - 1)) * ublas::prod(error_matrix, ublas::trans(error_matrix)) + ublas::identity_matrix<double>(error_matrix.size1()) * 0.00000000000001;
		
		// Compute akaike information criterion for comparison to other curve models.
		curve->aicc = akaike(curve->order + 1, 2 * size, rss);
	}
	
	// Probability of staying in the same state.
	double FeatureComparator::f0(double distance) {
		return exp(-1.0 / distance);
	}
	
	double FeatureComparator::f1(double distance1, double distance2) {
		if (distance1 == distance2)
			return (1.0 / distance1) * exp(-1.0 / distance1);
		else
			return (distance2 / (distance1 - distance2)) * (exp(-1.0 / distance1) - exp(-1.0 / distance2));
	}
	
	double FeatureComparator::f2(double distance2, double distance3) {
		if (distance2 == distance3)
			return 1.0 - (1.0 + 1.0 / distance2) * exp(-1.0 / distance2);
		else
			return 1.0 - exp(-1.0 / distance2) - (distance3 / (distance2 - distance3)) * (exp(-1.0 / distance2) - exp(-1.0 / distance3));
	}
	
	double FeatureComparator::forwardAlgorithm(ublas::matrix<double> emission_probabilities) {
		int states = emission_probabilities.size1();
		int observations = emission_probabilities.size2();
		
		ublas::matrix<double> alf = ublas::zero_matrix<double>(observations, states);
		ublas::matrix<double> alf_h = ublas::zero_matrix<double>(observations, states);
		
		ublas::vector<double> first_frame = ublas::matrix_column<ublas::matrix<double> >(emission_probabilities, 0);
		
		ublas::vector<double> row(prior.size());
		
		for (int i = 0; i < row.size(); i++)
			row[i] = prior[i] * first_frame[i];
		
		for (int i = 0; i < states; i++)
			alf(0, i) = row[i];
		
		ublas::vector<double> c(observations);
		c[0] = 0;
		
		for (int i = 0; i < alf.size2(); i++)
			c[0] += alf(0, i);
		
		if (c[0] == 0)
			return 0;
		else {
			for (int i = 0; i < states; i++)
				alf_h(0, i) = alf(0, i) / c[0];
			
			// Induction step.
			for (int t = 1; t < observations; t++) {
				ublas::vector<double> alf_h_row = ublas::matrix_row<ublas::matrix<double> >(alf_h, t - 1);
				ublas::vector<double> temp = ublas::prod(alf_h_row, transitions);
				
				for (int i = 0; i < alf.size2(); i++) {
					alf(t, i) = temp[i] * emission_probabilities(i, t);
				}
				
				c[t] = 0;
				
				for (int i = 0; i < alf.size2(); i++)
					c[t] += alf(t, i);
				
				if (c[t] == 0) {
					for (int i = 0; i < alf_h.size2(); i++)
						alf_h(t, i) = 0;
					
					break;
				} else {
					for (int i = 0; i < alf_h.size2(); i++)
						alf_h(t, i) = alf(t, i) / c[t];
				}
			}
			
			// Termination.
			double log_likelihood = 0;
			
			for (int i = 0; i < c.size(); i++) {
				if (c[i] != 0) {
					log_likelihood += log(c[i]);
				}
			}
			
			return log_likelihood;
		}
	}
	
	/*-----------------*
	 * Initialization. *
	 *-----------------*/
	
	// Savitzky-Golay smoothing of position and velocity vectors for feature trajectory.
	void FeatureComparator::smooth() {
		// Iteration variables.
		int padding = (savitzky_golay_f + 1) / 2;
		int history_size = feature->getHistorySize();
	
		// Fill two vectors with the first 2 differentiation filters of a 4th-order SG filter for 41 frames.
		ublas::vector<double> first;
		ublas::vector<double> second;
		fill_savitzky_golay(first, second);
		
		// Resize position and velocity vectors for filling.
		trajectory.resize(2, history_size);
		
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
			
			trajectory(0, index) = ublas::inner_prod(first, window);
			trajectory(1, index) = ublas::inner_prod(second, window);
		}
	}
	
	void FeatureComparator::fitCurve() {
		int size = trajectory.size2();
		vector<LeastSquaresParameters*> curves(3);
		ublas::vector<double>* concatonated_trajectory = new ublas::vector<double>(size * 2);
		
		for (int i = 0; i < size; i++) {
			(*concatonated_trajectory)[i] = trajectory(0, i);
			(*concatonated_trajectory)[i + size] = trajectory(1, i);
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
		
		// Precompute covariance determinant and inverse for reuse during comparison.
		bestFit->covarianceDeterminant = determinant(bestFit->covariance);
		bestFit->covarianceInverse = invert(bestFit->covariance);
		
		bestFit->mean = ublas::zero_matrix<double>(2, 2 * bestFit->order + 1);
		
		ublas::vector<double> positions = ublas::vector_range<ublas::vector<double> >(bestFit->trajectory, ublas::range(0, size));
		
		bestFit->xOffset = 0;
		
		for (int i = 0; i < size; i++)
			bestFit->xOffset += positions[i];
		
		bestFit->xOffset /= size;
		
		if (bestFit->order == 0) {
			bestFit->mean(0, 0) = bestFit->coefficients(0);				// Position state is constant.
			bestFit->mean(1, 0) = 0;									// Velocity state is 0.
			
			bestFit->timeSamples = ublas::zero_vector<double>(1);	
		} else if (bestFit->order == 1) {
			double midpoint = round(double(size - 1) / 2.0);
			
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
				vertex = round(double(size) / 2.0);
			
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
			bestFit->mean(1, 1) = 2 * bestFit->coefficients[0] * (midpoint1 + 1) + bestFit->coefficients[1];
			bestFit->mean(1, 2) = 2 * bestFit->coefficients[0] * (vertex + 1) + bestFit->coefficients[1];
			bestFit->mean(1, 3) = 2 * bestFit->coefficients[0] * (midpoint2 + 1) + bestFit->coefficients[1];
			bestFit->mean(1, 4) = 2 * bestFit->coefficients[0] * size + bestFit->coefficients[1];
			
			bestFit->timeSamples.resize(4);
			bestFit->timeSamples[0] = midpoint1;
			bestFit->timeSamples[1] = vertex - midpoint1;
			bestFit->timeSamples[2] = midpoint2 - vertex;
			bestFit->timeSamples[3] = size - 1 - midpoint2;
		}
	}
		
	void FeatureComparator::createHMM() {
		// Create the prior state.
		ublas::matrix<double> identity = ublas::identity_matrix<double>(2 * bestFit->order + 1);
		prior = ublas::matrix_row<ublas::matrix<double> >(identity, 0);
		
		// Create the transition probability matrix between states.
		if (bestFit->order == 0)
			transitions = ublas::scalar_matrix<double>(1, 1, 1.0);
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
			transitions(2, 2) = f0(bestFit->timeSamples[2]);
			transitions(2, 3) = f1(bestFit->timeSamples[2], bestFit->timeSamples[3]);
			transitions(2, 4) = f2(bestFit->timeSamples[2], bestFit->timeSamples[3]);
			transitions(3, 3) = f0(bestFit->timeSamples[3]);
			transitions(3, 4) = 1 - transitions(3, 3);
			transitions(4, 4) = 1;
		}
	}

	/*------------------------------*
	 * Savitzky-Golay trajectories. *
	 *------------------------------*/

	ublas::matrix<double> FeatureComparator::getTrajectory() {
		return trajectory;
	}
	
	/*---------------------------*
	 * Curve fitting attributes. *
	 *---------------------------*/
	
	ublas::matrix<double> FeatureComparator::getCovariance() {
		return bestFit->covariance;
	}
	
	ublas::matrix<double> FeatureComparator::getCovarianceInverse() {
		return bestFit->covarianceInverse;
	}
	
	double FeatureComparator::getCovarianceDeterminant() {
		return bestFit->covarianceDeterminant;
	}

	/*-----------------*
	 * HMM attributes. *
	 *-----------------*/

	ublas::vector<double> FeatureComparator::getPrior() {
		return prior;
	}

	ublas::matrix<double> FeatureComparator::getTransitions() {
		return transitions;
	}
	
	/*-------------*
	 * Operations. *
	 *-------------*/

	void FeatureComparator::initialize() {
		smooth();
		fitCurve();
		createHMM();
		 
		cout << "\t\tprior: " << prior << endl;
		cout << "\t\ttransitions: " << transitions << endl;
		cout << "\t\ttimeSamples: " << bestFit->timeSamples << endl;
		cout << "\t\tmean: " << bestFit->mean << endl;
		cout << "\t\tcovariance: " << bestFit->covariance << endl;
	}
	
	double FeatureComparator::compare(FeatureComparator* model) {
		ublas::matrix<double> other_trajectory = model->getTrajectory();
		
		int states = prior.size();
		int frames = other_trajectory.size2();
		
		// Compute emission probabilities for the feature trajectory.
		ublas::matrix<double> emission_probabilities(states, frames);
		
		bool singular = (bestFit->covarianceInverse.size1() == bestFit->covarianceInverse.size2() == 1 && bestFit->covarianceInverse(0, 0) == 0);
		
		if (!singular) {
			for (int i = 0; i < states; i++) {
				for (int j = 0; j < frames; j++) {
					ublas::vector<double> frame = ublas::matrix_column<ublas::matrix<double> >(other_trajectory, j);
					ublas::vector<double> frame_mean = ublas::matrix_column<ublas::matrix<double> >(bestFit->mean, i);
					
					ublas::vector<double> deviation = frame - frame_mean;
					
					double constant = 1.0 / (2 * PI * sqrt(bestFit->covarianceDeterminant));
					double exponential = exp(-0.5 * ublas::inner_prod(ublas::prod(ublas::trans(deviation), bestFit->covarianceInverse), deviation));
					
					emission_probabilities(i, j) = constant * exponential;
				}
			}
			
			return forwardAlgorithm(emission_probabilities) / double(frames);
		} else {
			cout << "singular!" << endl;
			return 0;
		}
	}
}