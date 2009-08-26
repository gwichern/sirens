/*
	Copyright 2008, 2009 Brandon Mechtley

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


#ifndef SOUND_H
#define SOUND_H

#include <vector>
#include <string>
using namespace std;

#include <sndfile.h>

#include "FeatureSet.h"

namespace Sirens {
	class Sound {
	protected:
		double hopLength, frameLength;
		
		string path;
		SNDFILE* soundFile;
		SF_INFO soundInfo;
		
		FeatureSet* features;
		
	public:
		Sound();
		Sound(string path_in);
		~Sound();
		
		// I/O.
		void open(string path_in);
		void close();
		
		// Basic sound information.
		int getSampleCount();
		int getSampleRate();
		int getChannels();
		double getHopLength();
		double getFrameLength();
		void setHopLength(double hop_length);
		void setFrameLength(double frame_length);
		string getPath();
				
		// Calculated sound information.
		int getSamplesPerFrame();
		int getSamplesPerHop();
		int getFrameCount();
		int getFFTSize();
		int getSpectrumSize();
		
		// Features.
		FeatureSet* getFeatures();
		void setFeatures(FeatureSet* new_features);
		void extractFeatures();		
	};
}

#endif
