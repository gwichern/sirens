#include "Feature.h"

#include "support/math_support.h"

namespace Sirens {
	Feature::Feature(int history_size) {		
		history = new CircularArray(history_size);
		segmentationParameters = new SegmentationParameters();
		max = 1.0;
		min = 0.0;
	}
	
	Feature::~Feature() {
		delete history;
		delete segmentationParameters;
	}
	
	/*--------------*
	 * Calculation. *
	 *--------------*/
	
	void Feature::calculate(CircularArray* input_in) {
		input = input_in;
		
		thread.start(run_feature, (void*)this);
	}
	
	void Feature::prepareCalculation() {
		if (!initialized) {
			freeMemory();
			initialize();
			
			initialized = true;
		}
		
		performCalculation();
		value = lerp(value, min, max);
		history->addValue(value);
	}
	
	void Feature::waitForCompletion() {
		thread.wait();
	}
	
	double Feature::getValue() {
		return value;
	}
	
	/*----------*
	 * History. *
	 *----------*/
	
	CircularArray* Feature::getHistory() {
		return history;
	}
	
	double Feature::getHistoryFrame(int i) {
		return history->getValue(i);
	}
	
	int Feature::getHistorySize() {
		return history->getSize();
	}
	
	int Feature::getMaxHistorySize() {
		return history->getMaxSize();
	}
	
	void Feature::setMaxHistorySize(int history_size) {
		CircularArray* new_history = new CircularArray(history_size);
		
		if (history) {
			for (int i = 0; i < history->getSize(); i++)
				history->addValue(history->getValue(i));
				
			delete history;
		}
		
		history = new_history;
	}
	
	/*----------------*
	 * Normalization. *
	 *----------------*/
	
	void Feature::setMax(double value) {
		max = value;
	}
	
	void Feature::setMin(double value) {
		min = value;
	}
	
	double Feature::getMax() {
		return max;
	}
	
	double Feature::getMin() {
		return min;
	}
	
	/*---------------*
	 * Segmentation. *
	 *---------------*/
	
	SegmentationParameters* Feature::getSegmentationParameters() {
		return segmentationParameters;
	}
	
	/*------------------------*
	 * String representation. *
	 *------------------------*/
	
	string Feature::toString() {
		return "Feature";
	}
	
	string Feature::historyString() {
		return toString() + "\n\t" + history->toString();
	}
}
