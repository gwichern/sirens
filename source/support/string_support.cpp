/*
 *  string_support.cpp
 *  SIRENS
 *
 *  Created by Brandon Mechtley on 8/7/07.
 *  Copyright 2007 Brandon Mechtley. All rights reserved.
 *
 */

#include <sstream>
using namespace std;

#include "string_support.h"

/*
 * tokenise - tokenise a string into a vector of token strings.
 *		str - input string.
 *		tokens - the vector of tokens, where the result goes
 *		delimeters - a string of character-length delimeters
 */
 
void tokenise(const string& str, vector<string>& tokens, const string& delimiters = " ") {
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);		// Skip delimiters at beginning.
    string::size_type pos = str.find_first_of(delimiters, lastPos);			// Find first non-delimiter.


    while (string::npos != pos || string::npos != lastPos) {
        tokens.push_back(str.substr(lastPos, pos - lastPos));		        // Found a token, add it to the vector.
        lastPos = str.find_first_not_of(delimiters, pos);					// Skip delimiters.  Note the not_of.
        pos = str.find_first_of(delimiters, lastPos);				        // Find next non-delimiter.
    }
}

/*
 * double_to_string - convert a double to a string.
 *		data - the double to convert to a string.
 *
 *		Note: string streams can cause major memory leaks when used frequently in Visual Studio 2005 SP1. 
 */
 
string double_to_string(double data) {
	stringstream ss;
	ss << data;
	return ss.str();
}

/*
 * int_to_string - convert an integer to a string.
 *		data - the integer to convert to a string.
 *
 *		Note: string streams can cause major memory leaks when used frequently in Visual Studio 2005 SP1. 
 */
 
string int_to_string(int data) {
	stringstream ss;
	ss << data;
	return ss.str();
}
