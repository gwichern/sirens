/*
	Copyright 2007, 2009 Brandon Mechtley, Gordon Wichern, Alex Fink, and Harvey Thornburg
	
	This file is part of Sirens.

	Sirens is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Sirens is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Sirens.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LOUDNESS_H
#define LOUDNESS_H

#include "../Feature.h"

#include <string>
using namespace std;

namespace Sirens {
	class Loudness : public Feature {
	public:	
		Loudness(int history_size = 1) : Feature(history_size) {}
		
		virtual void performCalculation();
		virtual string toString();
	};
}

#endif
