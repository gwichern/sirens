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
