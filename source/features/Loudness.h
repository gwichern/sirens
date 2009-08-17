#ifndef LOUDNESS_H
#define LOUDNESS_H

#include "../Feature.h"

#include <string>
using namespace std;

namespace Sirens {
	class Loudness : public Feature {
	public:	
		Loudness(int history_size = 1) : Feature(history_size) {}
		
		virtual void performCalculation();
		virtual string toString();
	};
}

#endif
