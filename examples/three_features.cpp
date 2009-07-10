#include <iostream>
using namespace std;

#include "sirens/Sound.h"
#include "sirens/features/Loudness.h"
#include "sirens/features/TemporalSparsity.h"
#include "sirens/features/SpectralSparsity.h"
using namespace Sirens;

int main() {
	// Initialize sound.
	Sound* sound = new Sound();
	sound->setFrameLength(0.04);
	sound->setHopLength(0.02);
	
	sound->open("sound.wav");
	
	// Initialize features.
	Loudness* loudness = new Loudness();
	TemporalSparsity* temporal_sparsity = new TemporalSparsity();
	SpectralSparsity* spectral_sparsity = new SpectralSparsity();
	
	loudness->setMaxHistorySize(sound->getFrameCount());
	temporal_sparsity->setMaxHistorySize(sound->getFrameCount());
	spectral_sparsity->setMaxHistorySize(sound->getFrameCount());
	
	// Set feature lists.
	vector<Feature*> sample_features;
	sample_features.push_back(loudness);
	sample_features.push_back(temporal_sparsity);
	
	vector<Feature*> spectral_features;
	spectral_features.push_back(spectral_sparsity);
	
	sound->setSampleFeatures(sample_features);
	sound->setSpectralFeatures(spectral_features);
	
	// Extract features.
	sound->extractFeatures();
	
	// Display feature histories.
	for (int i = 0; i < sample_features.size(); i++) {
		for (int j = 0; j < sample_features[i]->getHistorySize(); j++)
			cout << sample_features[i]->getHistoryFrame(j) << ", ";
		
		cout << endl;
	}
	
	for (int i = 0; i < spectral_sparsity->getHistorySize(); i++)
		cout << spectral_sparsity->getHistoryFrame(i) << ", ";
	
	cout << endl;
	
	// Clean up.
	delete sound;
	delete loudness;
	delete temporal_sparsity;
	delete spectral_sparsity;
	
	return 0;
}
