#include <iostream>
using namespace std;

#include "../source/Sirens.h"
using namespace Sirens;

int main() {
	// Open sound.
	Sound* sound = new Sound();
	sound->setFrameLength(0.04);
	sound->setHopLength(0.02);
	sound->open("sound.wav");
	
	// Initialize features.
	Loudness* loudness = new Loudness(sound->getFrameCount());
	TransientIndex* transient_index = new TransientIndex(sound->getFrameCount(), sound->getSpectrumSize(), sound->getSampleRate(), 15, 30);
	TemporalSparsity* temporal_sparsity = new TemporalSparsity(sound->getFrameCount(), 50);
	SpectralSparsity* spectral_sparsity = new SpectralSparsity(sound->getFrameCount());
	SpectralCentroid* spectral_centroid = new SpectralCentroid(sound->getFrameCount(), sound->getSpectrumSize(), sound->getSampleRate());
	Harmonicity* harmonicity = new Harmonicity(sound->getFrameCount(), sound->getSpectrumSize(), sound->getSampleRate());
	harmonicity->setAbsThreshold(1);
	harmonicity->setThreshold(0.1);
	harmonicity->setSearchRegionLength(5);
	harmonicity->setMaxPeaks(3);
	harmonicity->setLPFCoefficient(0.7);
	
	FeatureSet* features = new FeatureSet();
	features->addSampleFeature(loudness);
	features->addSampleFeature(temporal_sparsity);
	features->addSpectralFeature(spectral_sparsity);
	features->addSpectralFeature(spectral_centroid);
	features->addSpectralFeature(transient_index);
	features->addSpectralFeature(harmonicity);
	
	// Extract features.
	sound->setFeatures(features);
	sound->extractFeatures();
	sound->close();
	
	// Save features.
	features->saveCSV("features.csv");
	
	// Clean up.
	delete sound;
	delete features;
	delete loudness;
	delete harmonicity;
	delete transient_index;
	delete temporal_sparsity;
	delete spectral_sparsity;
	delete spectral_centroid;
	
	return 0;
}
