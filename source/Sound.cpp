#include "Sound.h"

#include <fstream>
using namespace std;

#include "FFT.h"
#include "CircularArray.h"
#include "math_support.h"

namespace Sirens {
	Sound::Sound() {
		frameLength = 0.04;
		hopLength = 0.02;
		
		path = "";
	}
	
	Sound::Sound(string path_in) {
		frameLength = 0.04;
		hopLength = 0.02;
		
		open(path_in);
	}
	
	Sound::~Sound() {
		file->closeFile();
		delete file;
	}
	
	/*
	 * Sound information.
	 */
	
	double Sound::getHopLength() {
		return hopLength;
	}
	
	double Sound::getFrameLength() {
		return frameLength;
	}
	
	int Sound::getSampleCount() {
		return file->getSize();
	}
	
	int Sound::getSamplesPerFrame() {
		return int(frameLength * double(getSampleRate()));
	}
	
	int Sound::getSamplesPerHop() {
		return int(hopLength * double(getSampleRate()));
	}
	
	int Sound::getFrameCount() {
		int frames = getSampleCount() / getSamplesPerHop();
		
		if (getSampleCount() % getSamplesPerHop() < getSamplesPerFrame())
			frames --;
		
		return frames;
	}
	
	int Sound::getSampleRate() {
		return int(file->getFileRate());
	}
	
	int Sound::getFFTSize() {
		return next_pow(getSamplesPerFrame(), 2);
	}
	
	int Sound::getSpectrumSize() {
		return getFFTSize() / 2 + 1;
	}
	
	void Sound::setHopLength(double hop_length) {
		hopLength = hop_length;
	}
	
	void Sound::setFrameLength(double frame_length) {
		frameLength = frame_length;
	}
	
	string Sound::getPath() {
		return path;
	}
	
	/*
	 * Sound processing.
	 */
	
	void Sound::open(string path_in) {
		path = path_in;
		
		file = new FileWvIn(path, false, true, 100000000, 1024);
	}
	
	void Sound::extractFeatures() {
		CircularArray sample_array(getSamplesPerFrame());			// Samples of the current frame.
		CircularArray windowed_array(getSamplesPerFrame(), true);	// Windowed samples of the current frame. Zero-padded for FFT.
		CircularArray spectrum_array(getSpectrumSize());			// Spectrum magnitudes of the current frame.
		
		// Prepare FFT.
		FFT fft(getFFTSize(), fftThreads, windowed_array.getData());
		
		double* window = create_hamming_window(getSamplesPerFrame());
		
		// Add each sample to the circular buffer.
		for (int i = 0; i < getSampleCount(); i++) {						
			// Calculate if we are at a new frame.
			if ((i % getSamplesPerHop()) == 0 && sample_array.getSize() >= getSamplesPerFrame()) {	
				// Loudness-based features.
				for (unsigned int j = 0; j < sampleFeatures.size(); j++)
					sampleFeatures[j]->calculate(&sample_array);
				
				for (unsigned int j = 0; j < sampleFeatures.size(); j++)
					sampleFeatures[j]->waitForCompletion();
				
				// FFT.
				for (int j = 0; j < getSamplesPerFrame(); j++)
					windowed_array.addValue(sample_array.getValueAt(j) * window[j]);
				
				fft.calculate();
								
				for (int j = 0; j < fft.getOutputSize(); j++)
					spectrum_array.addValue(sqrt(fft.getOutput()[j][0] * fft.getOutput()[j][0] + fft.getOutput()[j][1] * fft.getOutput()[j][1]));
				
				// Spectral and cepstral features.
				for (unsigned int j = 0; j < spectralFeatures.size(); j++)
					spectralFeatures[j]->calculate(&spectrum_array);
				
				for (unsigned int j = 0; j < spectralFeatures.size(); j++)
					spectralFeatures[j]->waitForCompletion();
			}
			
			// Add the sample to the buffer.
			sample_array.addValue(file->tick());
		}
		
		// Cleanup.
		delete [] window;
	}
	
	void Sound::saveFeaturesCSV(string csv_path) {
		fstream file(csv_path.c_str(), ios::out);
		
		for (int i = 2; i < getFrameCount(); i++) {
			for (unsigned int j = 0; j < sampleFeatures.size(); j++)
				file << sampleFeatures[j]->getHistory()->getData()[i] << ",";
				
			for (unsigned int j = 0; j < spectralFeatures.size(); j++)
				file << spectralFeatures[j]->getHistory()->getData()[i] << ",";
			
			file << i << endl;
		}
		
		file.close();
	}
	/*
	 * Feature list manipulations.
	 */
	
	void Sound::setSpectralFeatures(vector<Feature*> features) {
		spectralFeatures = features;
	}
	
	void Sound::setSampleFeatures(vector<Feature*> features) {
		sampleFeatures = features;
	}
	
	vector<Feature*> Sound::getSpectralFeatures() {
		return spectralFeatures;
	}
	
	vector<Feature*> Sound::getSampleFeatures() {
		return sampleFeatures;
	}
	
	Feature* Sound::getSpectralFeatureAt(int i) {
		return spectralFeatures[i];
	}
	
	Feature* Sound::getSampleFeatureAt(int i) {
		return sampleFeatures[i];
	}
}
