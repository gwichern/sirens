#ifndef SPECTRALSPARSITYFEATURE_H
#define SPECTRALSPARSITYFEATURE_H

#include "Feature.h"

#include <string>
using namespace std;

namespace Sirens {
	class SpectralSparsityFeature : public Feature {
	public:	
		SpectralSparsityFeature(int history_size = 1) : Feature(history_size) {}
		
		virtual void performCalculation();
		virtual string toString();
	};
}

#endif
