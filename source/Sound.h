/*
	Copyright 2008, 2009 Brandon Mechtley
	
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
		int chanOption;  //index of channel in .wav file to be examined (zero takes average between all channels)
		
		string path;
		SNDFILE* soundFile;
		SF_INFO soundInfo;
		
		FeatureSet* featureSet;
		
	public:
		Sound();
		Sound(string path_in);
		~Sound();
		
		// I/O.
		bool open(string path_in);
		void close();
		
		// Basic sound information.
		int getSampleCount();
		int getSampleRate();
		int getChannels();
		double getHopLength();
		double getFrameLength();
		void setHopLength(double hop_length);
		void setFrameLength(double frame_length);
		void setChanOption(int chan_option);
		string getPath();
				
		// Calculated sound information.
		int getSamplesPerFrame();
		int getSamplesPerHop();
		int getFrameCount();
		int getFFTSize();
		int getSpectrumSize();
		
		// Features.
		FeatureSet* getFeatureSet();
		void setFeatureSet(FeatureSet* feature_set);
		void extractFeatures();		
	};
}

#endif
