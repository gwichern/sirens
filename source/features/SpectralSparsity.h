#ifndef SPECTRALSPARSITY_H
#define SPECTRALSPARSITY_H

#include "Feature.h"

#include <string>
using namespace std;

namespace Sirens {
	class SpectralSparsity : public Feature {
	public:	
		SpectralSparsity(int history_size = 1) : Feature(history_size) {}
		
		virtual void performCalculation();
		virtual string toString();
	};
}

#endif
