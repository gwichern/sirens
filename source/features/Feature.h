#ifndef FEATURE_H
#define FEATURE_H

#include <string>
using namespace std;

#include "../stk/Thread.h"
using namespace stk;

#include "../CircularArray.h"

// No need for a mutex, since we are guaranteed to only access data when thread is complete.
namespace Sirens {
	class Feature {
	private:
		Thread thread;
		
	protected:
		bool initialized;
		
		CircularArray* input, *history;
		double value;
			
		virtual void initialize() {};
		virtual void freeMemory() {};
			
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
		
		string historyString();
	};
	
	static inline THREAD_RETURN THREAD_TYPE StartFeatureThread(void* user) {
		Feature* feature = (Feature*)user;
		feature->prepareCalculation();
		
		return 0;
	}
}

#endif
