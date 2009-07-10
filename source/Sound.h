#ifndef SOUND_H
#define SOUND_H

#include <vector>
#include <string>
using namespace std;

#include "stk/FileWvIn.h"
using namespace stk;

#include "features/Feature.h"

namespace Sirens {
	class Sound {
	protected:
		double hopLength, frameLength;
		int fftThreads;
		
		// File information.
		string path;
		FileWvIn* file;
		
		// Features.
		vector<Feature*> spectralFeatures;
		vector<Feature*> sampleFeatures;
		
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
		void extractFeatures();
		void saveFeaturesCSV(string csv_path);
		
		// Feature list manipulations.
		void setSpectralFeatures(vector<Feature*> features);
		void setSampleFeatures(vector<Feature*> features);
		
		vector<Feature*> getSpectralFeatures();
		vector<Feature*> getSampleFeatures();
		
		void addSpectralFeature(Feature* feature);
		void addSampleFeature(Feature* feature);
		
		void clearSpectralFeatures();
		void clearSampleFeatures();
	};
}

#endif
