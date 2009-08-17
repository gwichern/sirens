#ifndef TEMPORALSPARSITY_H
#define TEMPORALSPARSITY_H

#include "../Feature.h"

#include <string>
using namespace std;

namespace Sirens {
	class TemporalSparsity : public Feature {
	private:
		CircularArray* rmsWindow;
		
	public:	
		TemporalSparsity(int history_size = 1, int window_size = 50);
		~TemporalSparsity();
		
		void setWindowSize(int window_size);
		int getWindowSize();
		
		virtual void performCalculation();
		virtual string toString();
	};
}

#endif
