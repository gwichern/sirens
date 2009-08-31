/*
	Copyright 2007, 2008, 2009 Brandon Mechtley
	
	This file is part of Sirens.
	
	Sirens is free software: you can redistribute it and/or modify it under the terms 
	of the GNU Lesser General Public License as  published by the Free Software 
	Foundation, either version 3 of the License, or (at your option) any later version.
	
	Sirens is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
	PURPOSE.  See the GNU General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License along
	with Sirens. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __STRING_SUPPORT_H__
#define __STRING_SUPPORT_H__

#include <vector>
#include <string>
using namespace std;

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
using namespace boost::numeric;

namespace Sirens {
	void tokenise(const string& str, vector<string>& tokens, const string& delimiters);
	string double_to_string(double data);
	string int_to_string(int data);
	
	// Vectors.
	void write_csv_file(string path, vector<double> input, bool row = false);
	void write_csv_file(string path, ublas::vector<double> input, bool row = false);
	void write_csv_file(string path, double* input, int size, bool row = false);
	
	// Matrices.
	void write_csv_file(string path, vector<vector<double> > input);
	void write_csv_file(string path, ublas::matrix<double> input);
}

#endif
