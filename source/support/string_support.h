/*
 *  string_support.h
 *  SIRENS
 *
 *  Created by Brandon Mechtley on 8/7/07.
 *  Copyright 2007 Brandon Mechtley. All rights reserved.
 *
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
}

#endif
