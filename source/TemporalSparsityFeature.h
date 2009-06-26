#ifndef TEMPORALSPARSITYFEATURE_H
#define TEMPORALSPARSITYFEATURE_H

#include "Feature.h"

#include <string>
using namespace std;

namespace Sirens {
	class TemporalSparsityFeature : public Feature {
	private:
		CircularArray* rmsWindow;
		
	public:	
		TemporalSparsityFeature(int history_size = 1, int window_size = 50);
		~TemporalSparsityFeature();
		
		void setWindowSize(int window_size);
		
		virtual void performCalculation();
		virtual string toString();
	};
}

#endif
