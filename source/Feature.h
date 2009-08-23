#ifndef FEATURE_H
#define FEATURE_H

#include <pthread.h>
#include <string>
using namespace std;

#include "Thread.h"
#include "CircularArray.h"
#include "SegmentationParameters.h"

namespace Sirens {
	class Feature : public Thread {
	private:
		// Concurrency.
		bool running;								// whether or not the thread is running. for exiting the run loop.
		bool recalculate;
		bool complete;
		
		pthread_mutex_t mutex;
		pthread_cond_t recalculateCondition;
		pthread_cond_t completeCondition;					
		
		// Segmentation.
		SegmentationParameters* segmentationParameters;
		
		// History of every feature value calculated.
		CircularArray* history;
		
		// Normalization. Values are scaled between min and max.
		double min, max;
		
	protected:
		CircularArray* input;						// input buffer to perform calculations on.
		bool initialized;							// whether or not the feature has been initialized.
		double value;								// actual value calculated.
			
		virtual void initialize() {};				// on-demand initialization, as some parameters may not be set on construction.
		virtual void freeMemory() {};				// called on re-initialization, to free up any memory allocated at initialization.
		
	public:
		Feature(int history_size = 1);
		virtual ~Feature();
		
		void run();									// waits for signal to calculate, calculates, loops.
		void stop();								// turns off the run loop. Does not destroy the thread.
		
		// Calculation.
		void calculate(CircularArray* input_in);
		void prepareCalculation();
		virtual void performCalculation() {}		// actual implementation.
		void waitForCompletion();					// blocks until the calculation is complete.
		double getValue();
		
		// Access to feature value history.
		CircularArray* getHistory();
		double getHistoryFrame(int i);
		int getHistorySize();
		int getMaxHistorySize();
		void setMaxHistorySize(int history_size);
		
		// Normalization.
		void setMax(double value);
		void setMin(double value);
		double getMax();
		double getMin();
		
		// Segmentation.
		SegmentationParameters* getSegmentationParameters();
		
		// String representation.
		virtual string toString();
		string historyString();	
	};
}

#endif
