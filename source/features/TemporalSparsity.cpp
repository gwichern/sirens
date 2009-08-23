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
		
		if (rmsWindow->getSize() >= rmsWindow->getMaxSize()) {
			double max = 0;
			double sum = 0;
			
			double* rms_item = rmsWindow->getData();
			int rms_size = rmsWindow->getSize();
			
			for (int i = 0; i < rms_size; i++) {
				max = maximum(max, (*rms_item));
				sum += (*rms_item);
				
				rms_item ++;
			}
			
			value = max / sum;
		
		} else
			value = 0;
	}	

	string TemporalSparsity::toString() {
		return string("Temporal Sparsity");
	}
}
