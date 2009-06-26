#include <ctime>
#include <string>
#include <iostream>
using namespace std;

// Sound processing
#include "Sound.h"

// Features
#include "Feature.h"
#include "LoudnessFeature.h"
#include "HarmonicityFeature.h"
#include "TransientIndexFeature.h"
#include "TemporalSparsityFeature.h"
#include "SpectralSparsityFeature.h"
#include "SpectralCentroidFeature.h"
using namespace Sirens;

int main(int argc, char** argv) {
	Sound sound;
	sound.setHopLength(0.02);
	sound.setFrameLength(0.04);
	sound.open("/Users/brandon/Documents/Repositories/soundwalk/test/test.wav");
	
	int frames = sound.getFrameCount();
	int spectrum_size = sound.getSpectrumSize();
	int sample_rate = sound.getSampleRate();
	string path = sound.getPath();
	
	cout << "Sirens::Sound" << endl <<
		"\tPath: " << sound.getPath() << endl <<
		"\tSamples: " << sound.getSampleCount() << endl <<
		"\tSample rate: " << sample_rate << endl <<
		"\tFrame length: " << sound.getFrameLength() << "s (" << sound.getSamplesPerFrame() << " samples)" << endl <<
		"\tHop length: " << sound.getHopLength() << "s (" << sound.getSamplesPerHop() << " samples)" << endl <<
		"\tFFT size: " << sound.getFFTSize() << endl <<
		"\tSpectrum size: " << sound.getSpectrumSize() << endl << endl;
	
	vector<Feature*> sample_features;
	vector<Feature*> spectral_features;
	
	LoudnessFeature* loudness = new LoudnessFeature();
	TemporalSparsityFeature* temporal_sparsity = new TemporalSparsityFeature();
	SpectralSparsityFeature* spectral_sparsity = new SpectralSparsityFeature();
	SpectralCentroidFeature* spectral_centroid = new SpectralCentroidFeature();
	TransientIndexFeature* transient_index = new TransientIndexFeature();
	HarmonicityFeature* harmonicity = new HarmonicityFeature();
	
	sample_features.push_back(loudness);
	sample_features.push_back(temporal_sparsity);
	
	spectral_features.push_back(spectral_sparsity);
	spectral_features.push_back(spectral_centroid);
	spectral_features.push_back(transient_index);
	spectral_features.push_back(harmonicity);
	
	for (int i = 0; i < sample_features.size(); i++)
		sample_features[i]->setHistorySize(frames);
	
	for (int i = 0; i < spectral_features.size(); i++)
		spectral_features[i]->setHistorySize(frames);
	
	spectral_centroid->setSampleRate(sample_rate);
	spectral_centroid->setSpectrumSize(spectrum_size);
	
	transient_index->setSampleRate(sample_rate)
	transient_index->setSpectrumSize(spectrum_size);
	transient_index->setMels(15);
	transient_index->setFilters(30);
	
	harmonicity->setSampleRate(sample_rate);
	harmonicity->setSpectrumSize(spectrum_size);
	harmonicity->setAbsThreshold(1);
	harmonicity->setThreshold(0.1);
	harmonicity->setSearchRegionLength(5);
	harmonicity->setMaxPeaks(3);
	harmonicity->setLPFCoefficient(0.7);
	
	sound.setSampleFeatures(sample_features);
	sound.setSpectralFeatures(spectral_features);
	
	clock_t t1 = clock();	
	
	sound.extractFeatures();
	
	clock_t t2 = clock();	
	cout << "Elapsed time: " << (double)(t2 - t1) / CLOCKS_PER_SEC << endl
	
	sound.saveFeaturesCSV("features.csv");
	
	for (int i = 0; i < sample_features.size(); i++)
		delete sample_features[i];
	
	for (int i = 0; i < spectral_features.size(); i++)
		delete spectral_features[i];
	
	return 0;
}
