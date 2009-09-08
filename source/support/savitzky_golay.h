#ifndef __SAVITZKYGOLAY_H__
#define __SAVITZKYGOLAY_H__

#include "boost/numeric/ublas/vector.hpp"
using namespace boost::numeric;

// First two differentiation filters for a 4th order Sazitzky-Golay filter for 41 frames
namespace Sirens {
	static const int savitzky_golay_k = 4;
	static const int savitzky_golay_f = 41;
	
	void fill_savitzky_golay(ublas::vector<double>& first, ublas::vector<double>& second);
}
#endif
