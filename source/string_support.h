/*
 *  string_support.h
 *  SIRENS
 *
 *  Created by Brandon Mechtley on 8/7/07.
 *  Copyright 2007 Brandon Mechtley. All rights reserved.
 *
 */

#pragma once

#include <vector>
#include <string>
using namespace std;

void tokenise(const string& str, vector<string>& tokens, const string& delimiters);
string double_to_string(double data);
string int_to_string(int data);
