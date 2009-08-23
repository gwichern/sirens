#include "Feature.h"

#include "support/math_support.h"

namespace Sirens {
	Feature::Feature(int history_size) {		
		history = new CircularArray(history_size);
		segmentationParameters = new SegmentationParameters();
		max = 1.0;
		min = 0.0;
		
		recalculate = false;
		complete = false;
		running = true;
		
		pthread_mutex_init(&mutex, NULL);
		pthread_cond_init(&recalculateCondition, NULL);
		pthread_cond_init(&completeCondition, NULL);
	}
	
	Feature::~Feature() {
		stop();
		
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&recalculateCondition);
		pthread_cond_destroy(&completeCondition);
		
		delete history;
		delete segmentationParameters;
	}
	
	void Feature::run() {		
		// Continue until no longer running.
		while (running) {			
			pthread_mutex_lock(&mutex);		
				
			if (!recalculate)
				pthread_cond_wait(&recalculateCondition, &mutex);
						
			recalculate = false;
			
			// If the thread stopped running during the wait, don't bother calculating anything. Just exit the loop.
			if (running) {
				prepareCalculation();
				history->addValue(value);
				
				complete = true;
				pthread_cond_signal(&completeCondition);
			}
			
			pthread_mutex_unlock(&mutex);
		}
	}
	
	void Feature::stop() {
		// Signal the thread to stop waiting for a new calculation so that it can stop.
		pthread_mutex_lock(&mutex);
		running = false;
		pthread_cond_signal(&recalculateCondition);
		
		pthread_mutex_unlock(&mutex);
	}
	
	/*--------------*
	 * Calculation. *
	 *--------------*/
	
	// Routine that users call to request a calculation.
	void Feature::calculate(CircularArray* input_in) {
		// Set the input buffer, so we have access to it.
		pthread_mutex_lock(&mutex);
		input = input_in;
		
		recalculate = true;
		
		pthread_cond_signal(&recalculateCondition);
		
		pthread_mutex_unlock(&mutex);
	}
	
	// Wraps performCalculation()--initializes the feature, if necessary, and normalizes the value.
	void Feature::prepareCalculation() {
		if (!initialized) {
			freeMemory();
			initialize();
			
			initialized = true;
		}
		
		performCalculation();
		value = lerp(value, min, max);
	}
	
	// Routine that users call to wait for the calculation to complete.
	void Feature::waitForCompletion() {
		pthread_mutex_lock(&mutex);
		
		if (!complete)
			pthread_cond_wait(&completeCondition, &mutex);
					
		complete = false;
		pthread_mutex_unlock(&mutex);
	}
	
	// Return the feature value.
	double Feature::getValue() {
		double return_value = -1;
		
		pthread_mutex_lock(&mutex);
		return_value = value;
		pthread_mutex_unlock(&mutex);
		
		return return_value;
	}
	
	/*----------*
	 * History. *
	 *----------*/
	
	// These don't need to be locked, because CircularArray is thread-safe, and the history pointer never changes.
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
