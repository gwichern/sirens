#ifndef LOUDNESSFEATURE_H
#define LOUDNESSFEATURE_H

#include "Feature.h"

#include <string>
using namespace std;

namespace Sirens {
	class LoudnessFeature : public Feature {
	public:	
		LoudnessFeature(int history_size = 1) : Feature(history_size) {}
		
		virtual void performCalculation();
		virtual string toString();
	};
}

#endif
