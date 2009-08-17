#include "Sound.h"

#include "FFT.h"
#include "CircularArray.h"
#include "support/math_support.h"

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
		return file->fileSize();
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
		
		file = new FileWvIn(path, false, true);
		
		while(file->tick())
			samples++;
		
		file->reset();
	}
	
	void Sound::close() {
		file->closeFile();
	}
	
	void Sound::extractFeatures() {
		CircularArray sample_array(getSamplesPerFrame());			// Samples of the current frame.
		CircularArray windowed_array(getSamplesPerFrame(), true);	// Windowed samples of the current frame. Zero-padded for FFT.
		CircularArray spectrum_array(getSpectrumSize());			// Spectrum magnitudes of the current frame.
		
		// Prepare FFT.
		FFT fft(getFFTSize(), windowed_array.getData());
		
		double* window = create_hamming_window(getSamplesPerFrame());
		
		// Add each sample to the circular buffer.
		for (int i = 0; i < getSampleCount(); i++) {
			// Calculate if we are at a new frame.
			if ((i % getSamplesPerHop()) == 0 && sample_array.getSize() >= getSamplesPerFrame()) {	
				if (((i / getSamplesPerHop()) % 1000) == 0)
					cout << "Frame " << i / getSamplesPerHop() << endl;
					
				// Calculate sample features that don't need FFT.
				features->calculateSampleFeatures(&sample_array);
				
				// FFT.
				for (int j = 0; j < getSamplesPerFrame(); j++)
					windowed_array.addValue(sample_array.getValueAt(j) * window[j]);
				
				fft.calculate();
								
				for (int j = 0; j < fft.getOutputSize(); j++)
					spectrum_array.addValue(sqrt(fft.getOutput()[j][0] * fft.getOutput()[j][0] + fft.getOutput()[j][1] * fft.getOutput()[j][1]));
				
				// Calculate spectral features.
				features->calculateSpectralFeatures(&spectrum_array);
			}
			
			// Add the sample to the buffer.
			sample_array.addValue(file->tick());
		}
		
		// Cleanup.
		delete [] window;
	}
	
	FeatureSet* Sound::getFeatures() {
		return features;
	}
	
	void Sound::setFeatures(FeatureSet* new_features) {
		features = new_features;
	}
}
