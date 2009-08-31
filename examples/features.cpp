/*
	Copyright 2009 Brandon Mechtley
	
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

#include "../source/Sirens.h"
using namespace Sirens;

int main() {
	// Open sound.
	Sound* sound = new Sound();
	sound->setFrameLength(0.04);
	sound->setHopLength(0.02);
	sound->open("sounds/whistle4.wav");
	
	// Output some information about the sound.
	cout << "whistle4.wav" << endl;
	cout << "\tDuration: " << double(sound->getSampleCount()) / sound->getSampleRate() << "s (" << sound->getSampleCount() << " samples)" << endl;
	cout << "\tSample rate: " << sound->getSampleRate() << " Hz" << endl;
	cout << "\tFrame length: " << sound->getFrameLength() << "s (" << sound->getSamplesPerFrame() << " samples)" << endl;
	cout << "\tHop length: " << sound->getFrameLength() << "s (" << sound->getSamplesPerHop() << " samples)" << endl;
	cout << "\tFFT size: " << sound->getFFTSize() << " samples" << endl;
	cout << "\tSpectrum size: " << sound->getSpectrumSize() << " bins" << endl;
	
	// Initialize features.
	int frames = sound->getFrameCount() - 1;			// First frame of TransientIndex is junk, so don't want to record it.
	int spectrum_size = sound->getSpectrumSize();
	int sample_rate = sound->getSampleRate();
	
	Loudness* loudness = new Loudness(frames);
	TransientIndex* transient_index = new TransientIndex(frames, spectrum_size, sample_rate, 30, 15);
	TemporalSparsity* temporal_sparsity = new TemporalSparsity(frames, 50);
	SpectralSparsity* spectral_sparsity = new SpectralSparsity(frames);
	SpectralCentroid* spectral_centroid = new SpectralCentroid(frames, spectrum_size, sample_rate);
	Harmonicity* harmonicity = new Harmonicity(frames, spectrum_size, sample_rate);
	harmonicity->setAbsThreshold(1);
	harmonicity->setThreshold(0.1);
	harmonicity->setSearchRegionLength(5);
	harmonicity->setMaxPeaks(3);
	harmonicity->setLPFCoefficient(0.7);
	
	// Initialize a feature set to hold all the features.
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
