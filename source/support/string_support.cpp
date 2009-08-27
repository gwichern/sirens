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

#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

#include "string_support.h"

namespace Sirens {
	void tokenise(const string& str, vector<string>& tokens, const string& delimiters = " ") {
		string::size_type lastPos = str.find_first_not_of(delimiters, 0);		// Skip delimiters at beginning.
		string::size_type pos = str.find_first_of(delimiters, lastPos);			// Find first non-delimiter.
		
		while (string::npos != pos || string::npos != lastPos) {
			tokens.push_back(str.substr(lastPos, pos - lastPos));		        // Found a token, add it to the vector.
			lastPos = str.find_first_not_of(delimiters, pos);					// Skip delimiters.  Note the not_of.
			pos = str.find_first_of(delimiters, lastPos);				        // Find next non-delimiter.
		}
	}

	string double_to_string(double data) {
		stringstream ss;
		ss << data;
		return ss.str();
	}
 
	string int_to_string(int data) {
		stringstream ss;
		ss << data;
		return ss.str();
	}
	
	void write_csv_file(string path, vector<vector<double> > input) {
		fstream file(path.c_str(), ios::out);
		
		for (int i = 0; i < input.size(); i++) {
			for (int j = 0; j < input[i].size(); j++) {
				file << input[i][j];
				
				if (j < input[i].size() - 1)
					file << ",";
			}
			
			file << endl;
		}
		
		file.close();
	}
	
	void write_csv_file(string path, vector<double> input, bool row) {
		fstream file(path.c_str(), ios::out);
		
		for (int i = 0; i < input.size(); i++) {
			file << input[i];
			
			if (i < input.size() - 1) {
				if (!row)
					file << endl;
				else
					file << ",";
			}
		}
		
		file << endl;
		
		file.close();
	}
	
	void write_csv_file(string path, ublas::vector<double> input, bool row) {
		vector<double> copy(input.size(), 0);
		
		for (int i = 0; i < input.size(); i++)
			copy[i] = input[i];
			
		write_csv_file(path, copy, row);
	}
	
	void write_csv_file(string path, ublas::matrix<double> input) {
		fstream file(path.c_str(), ios::out);
		
		for (int i = 0; i < input.size1(); i++) {
			for (int j = 0; j < input.size2(); j++) {
				file << input(i, j);
				
				if (j < input.size2() - 1)
					file << ",";
			}
			
			file << endl;
		}
		
		file.close();
	}
}
