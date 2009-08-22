#ifndef FEATURE_H
#define FEATURE_H

#include <string>
using namespace std;

#include "stk/Thread.h"
using namespace stk;

#include "CircularArray.h"
#include "SegmentationParameters.h"

// No need for a mutex, since we are guaranteed to only access data when thread is complete.
namespace Sirens {
	class Feature : Thread {
	private:
		Thread thread;
		
	protected:
		bool initialized;
		
		CircularArray* input, *history;
		double value, min, max;
			
		virtual void initialize() {};
		virtual void freeMemory() {};
		
		SegmentationParameters* segmentationParameters;
		
	public:
		Feature(int history_size = 1);
		virtual ~Feature();
		
		void calculate(CircularArray* input_in);
		void waitForCompletion();
		
		double getValue();
		CircularArray* getHistory();
		double getHistoryFrame(int i);
		
		virtual string toString();
		virtual void performCalculation() {}
		
		void prepareCalculation();
		
		void setMaxHistorySize(int history_size);
		int getMaxHistorySize();
		int getHistorySize();
		
		void setMax(double value);
		void setMin(double value);
		double getMax();
		double getMin();
		
		SegmentationParameters* getSegmentationParameters();
		
		string historyString();
	};
	
	static inline THREAD_RETURN THREAD_TYPE StartFeatureThread(void* user) {
		Feature* feature = (Feature*)user;
		feature->prepareCalculation();
		
		return 0;
	}
}

#endif
