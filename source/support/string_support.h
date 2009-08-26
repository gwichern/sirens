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

#ifndef STRING_SUPPORT_H
#define STRING_SUPPORT_H

#include <vector>
#include <string>
using namespace std;

namespace Sirens {
	void tokenise(const string& str, vector<string>& tokens, const string& delimiters);
	string double_to_string(double data);
	string int_to_string(int data);
	void write_csv_file(string path, vector<vector<double> > input);
	void write_csv_file(string path, vector<double> input, bool row = false);
}

#endif
