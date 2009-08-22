#include "Sound.h"

#include "FFT.h"
#include "CircularArray.h"
#include "support/math_support.h"
#include "support/string_support.h"

namespace Sirens {
	Sound::Sound() {
		frameLength = 0.04;
		hopLength = 0.02;
		
		soundFile = NULL;
		path = "";
	}
	
	Sound::Sound(string path_in) {
		frameLength = 0.04;
		hopLength = 0.02;
		soundFile = NULL;
		
		open(path_in);
	}
	
	Sound::~Sound() {
		close();
	}
	
	/*
	 * IO
	 */
	
	void Sound::open(string path_in) {
		path = path_in;
		
		if (!(soundFile = sf_open(path.c_str(), SFM_READ, &soundInfo))) {
			// Error;
		}
	}
	
	void Sound::close() {
		if (soundFile != NULL) {
			sf_close(soundFile);
			soundFile = NULL;
		}
	}
	
	/*
	 * Basic sound information.
	 */
	
	int Sound::getSampleCount() {
		return soundInfo.frames;
	}

	int Sound::getSampleRate() {
		return soundInfo.samplerate;
	}

	int Sound::getChannels() {
		return soundInfo.channels;
	}
	
	double Sound::getHopLength() {
		return hopLength;
	}
	
	double Sound::getFrameLength() {
		return frameLength;
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
	 * Calculated sound information.
	 */
	
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
	
	int Sound::getFFTSize() {
		return next_pow(getSamplesPerFrame(), 2);
	}
	
	int Sound::getSpectrumSize() {
		return getFFTSize() / 2 + 1;
	}
	
	/*
	 * Features.
	 */
	
	FeatureSet* Sound::getFeatures() {
		return features;
	}
	
	void Sound::setFeatures(FeatureSet* new_features) {
		features = new_features;
	}
	
	void Sound::extractFeatures() {
		CircularArray sample_array(getSamplesPerFrame());			// Samples of the current frame.
		CircularArray windowed_array(getSamplesPerFrame(), true);	// Windowed samples of the current frame. Zero-padded for FFT.
		CircularArray spectrum_array(getSpectrumSize());			// Spectrum magnitudes of the current frame.
		
		// Prepare FFT.
		FFT fft(getFFTSize(), windowed_array.getData());
		
		double* window = create_hamming_window(getSamplesPerFrame());
		
		// Add each sample to the circular buffer.
		int readcount;
 		long frame_number = 0;
		int samples_per_hop = getSamplesPerHop();
		double* new_samples = new double[samples_per_hop];
				
		while (readcount = sf_read_double(soundFile, new_samples, samples_per_hop)) {
			for (int i = 0; i < samples_per_hop; i++)
				sample_array.addValue(new_samples[i]);
			
			// The first hop or two will not necessarily be a full frame's worth of data.
			if (sample_array.getSize() == sample_array.getMaxSize()) {
				if (frame_number % 1000 == 0)
					cout << "Frame " << frame_number << endl;
				
				// Calculate sample features that don't need FFT.
				features->calculateSampleFeatures(&sample_array);
				
				// FFT.
				for (int i = 0; i < getSamplesPerFrame(); i++)
					windowed_array.addValue(sample_array.getValueAt(i) * window[i]);
				
				fft.calculate();
				
				for (int i = 0; i < fft.getOutputSize(); i++) {					
					double first = fft.getOutput()[i][0];
					double second = fft.getOutput()[i][1];
					
					spectrum_array.addValue(sqrt(first * first + second * second));
				}
				
				// Calculate spectral features.
				features->calculateSpectralFeatures(&spectrum_array);
				
				frame_number = frame_number + 1;
			}
		}
		
		// Cleanup.
		delete [] new_samples;
		delete [] window;
	}
}
