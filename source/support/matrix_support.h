#ifndef __MATRIX_SUPPORT_H__
#define __MATRIX_SUPPORT_H__

#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/vector_proxy.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/matrix_proxy.hpp"
#include "boost/numeric/ublas/triangular.hpp"
#include "boost/numeric/ublas/operation.hpp"
#include "boost/numeric/ublas/lu.hpp"
using namespace boost::numeric;

namespace Sirens {
	ublas::matrix<double> invert(ublas::matrix<double> input);
	double determinant(ublas::matrix<double> input);
	ublas::matrix<double> normalize_affinity(ublas::matrix<double> input);
}

#endif
