/*
	Copyright 2007, 2008, 2009 Brandon Mechtley, Gordon Wichern, and Jiachen Xue
	
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

#ifndef __SOUND_COMPARATOR__
#define __SOUND_COMPARATOR__

#include "FeatureComparator.h"

#include "../FeatureSet.h"

#include <vector>
using namespace std;

namespace Sirens {
	class SoundComparator {
	private:
		vector<FeatureComparator*> featureComparators;
		
	public:
		SoundComparator(FeatureSet* features_in);
		~SoundComparator();
		
		vector<FeatureComparator*> getFeatureComparators();
		
		double compare(SoundComparator* sound_comparator);
		
		void initialize();
	};
}

#endif
