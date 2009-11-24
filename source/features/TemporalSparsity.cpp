/*
	Copyright 2007, 2009 Brandon Mechtley, Gordon Wichern, Alex Fink, and Harvey Thornburg
	
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

#include "TemporalSparsity.h"

#include "../support/math_support.h"

namespace Sirens {
	TemporalSparsity::TemporalSparsity(int history_size, int window_size) : Feature(history_size) {
		rmsWindow = NULL;
		
		setWindowSize(window_size);
	}

	TemporalSparsity::~TemporalSparsity() {
		delete rmsWindow;
	}
	
	void TemporalSparsity::setWindowSize(int window_size) {	
		CircularArray* new_window = new CircularArray(window_size);
				
		if (rmsWindow) {
			for (int i = 0; i < rmsWindow->getSize(); i++)
				new_window->addValue(rmsWindow->getValue(i));
			
			delete rmsWindow;
		}
		
		rmsWindow = new_window;
	}
	
	int TemporalSparsity::getWindowSize() {
		return rmsWindow->getMaxSize();
	}
	
	void TemporalSparsity::performCalculation() {
		double rms = signal_rms(input);
		
		rmsWindow->addValue(rms);
		
		double max = 0;
		double sum = 0;
		value=0;
		
		double* rms_item = rmsWindow->getData();
		int rms_size = rmsWindow->getSize();
		
		for (int i = 0; i < rms_size; i++) {
			max = maximum(max, (*rms_item));
			sum += (*rms_item);
			
			rms_item ++;
		}
		
		if (sum) {
			if (rmsWindow->getSize() >= rmsWindow->getMaxSize())
				value = max / sum;
			else
				value = (double(rmsWindow->getSize()) / double(rmsWindow->getMaxSize())) * max / sum;
			
		}
	}	

	string TemporalSparsity::toString() {
		return string("Temporal Sparsity");
	}
}
