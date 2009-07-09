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
				new_window->addValue(rmsWindow->getValueAt(i));
			
			delete rmsWindow;
		}
		
		rmsWindow = new_window;
	}
	
	int TemporalSparsity::getWindowSize() {
		return rmsWindow->getSize();
	}
	
	void TemporalSparsity::performCalculation() {
		double rms = signal_rms(input->getData(), input->getSize());
		
		rmsWindow->addValue(rms);
	
		if (rmsWindow->getSize() >= rmsWindow->getMaxSize()) {
			double max = 0;
			double sum = 0;
			
			double* input_item = rmsWindow->getData();
			
			for (int i = 0; i < rmsWindow->getSize(); i++) {
				max = maximum(max, (*input_item));
				sum += (*input_item);
				
				input_item ++;
			}
		
			value = max / sum;
		
		} else
			value = 0;
	}	

	string TemporalSparsity::toString() {
		return string("Temporal Sparsity");
	}
}
