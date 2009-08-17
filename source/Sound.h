#ifndef SOUND_H
#define SOUND_H

#include <vector>
#include <string>
using namespace std;

#include "stk/FileWvIn.h"
using namespace stk;

#include "FeatureSet.h"

namespace Sirens {
	class Sound {
	protected:
		double hopLength, frameLength;
		
		// File information.
		string path;
		FileWvIn* file;
		long samples;
		
		// Features.
		FeatureSet* features;
		
	public:
		Sound();
		Sound(string path_in);
		~Sound();
		
		// Sound information.
		double getHopLength();
		double getFrameLength();
		
		int getSampleCount();
		int getSamplesPerFrame();
		int getSamplesPerHop();
		int getFrameCount();
		int getSampleRate();
		int getFFTSize();
		int getSpectrumSize();
		string getPath();
		
		void setHopLength(double hop_length);
		void setFrameLength(double frame_length);
		
		// Sound processing.
		void open(string path_in);
		void close();
		void extractFeatures();
		
		// Feature list manipulations.
		FeatureSet* getFeatures();
		void setFeatures(FeatureSet* new_features);
	};
}

#endif
