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

#include <iostream>
using namespace std;

#include "../source/Sirens.h"
#include "../source/support/matrix_support.h"
#include "../source/support/string_support.h"
using namespace Sirens;

#include <boost/numeric/ublas/io.hpp>

int main() {
	// Open sound.
	vector<string> files(4);
	files[0] = "sounds/whistle1.wav";
	files[1] = "sounds/whistle2.wav";
	files[2] = "sounds/whistle3.wav";
	files[3] = "sounds/whistle4.wav";
	
	// We have three of everything, because we have two files and (unfortunately) features can't be shared.
	vector<Sound*> sounds(files.size());
	vector<Loudness*> loudness(files.size());
	vector<TemporalSparsity*> temporal_sparsity(files.size());
	vector<SpectralSparsity*> spectral_sparsity(files.size());
	vector<SpectralCentroid*> spectral_centroid(files.size());
	vector<TransientIndex*> transient_index(files.size());
	vector<Harmonicity*> harmonicity(files.size());
	vector<FeatureSet*> feature_sets(files.size());
	vector<SoundComparator*> comparators(files.size());
	
	// Initialize sounds.
	for (int i = 0; i < files.size(); i++) {
		// Initialize the sound files.
		sounds[i] = new Sound();
		sounds[i]->setFrameLength(0.04);
		sounds[i]->setHopLength(0.02);
		sounds[i]->open(files[i]);
		
		// The first frame of TransientIndex is junk, so record everything except the first frame.
		int frames = sounds[i]->getFrameCount() - 1;
		
		int spectrum_size = sounds[i]->getSpectrumSize();
		int sample_rate = sounds[i]->getSampleRate();
		
		// Initialize the features.
		loudness[i] = new Loudness(frames);
		transient_index[i] = new TransientIndex(frames, spectrum_size, sample_rate, 30, 15);
		temporal_sparsity[i] = new TemporalSparsity(frames, 50);
		spectral_sparsity[i] = new SpectralSparsity(frames);
		spectral_centroid[i] = new SpectralCentroid(frames, spectrum_size, sample_rate);
		harmonicity[i] = new Harmonicity(frames, spectrum_size, sample_rate);
		harmonicity[i]->setAbsThreshold(1);
		harmonicity[i]->setThreshold(0.1);
		harmonicity[i]->setSearchRegionLength(5);
		harmonicity[i]->setMaxPeaks(3);
		harmonicity[i]->setLPFCoefficient(0.7);
		
		// Initialize the feature set.
		feature_sets[i] = new FeatureSet();
		feature_sets[i]->addSampleFeature(loudness[i]);
		feature_sets[i]->addSpectralFeature(spectral_centroid[i]);
		feature_sets[i]->addSpectralFeature(spectral_sparsity[i]);
		feature_sets[i]->addSampleFeature(temporal_sparsity[i]);
		feature_sets[i]->addSpectralFeature(transient_index[i]);
		feature_sets[i]->addSpectralFeature(harmonicity[i]);
		
		// Extract features.
		sounds[i]->setFeatures(feature_sets[i]);
		sounds[i]->extractFeatures();
		sounds[i]->close();
		
		comparators[i] = new SoundComparator(feature_sets[i]);
		comparators[i]->initialize();
	}
	
	ublas::matrix<double> likelihood(files.size(), files.size());
	
	
	// Compare each sound to itself and the other sound.
	for (int i = 0; i < files.size(); i++) {
		for (int j = 0; j < files.size(); j++)
			likelihood(i, j) = comparators[i]->compare(comparators[j]);
	}
	
	ublas::matrix<double> affinity = normalize_affinity(likelihood);
	
	cout << "Log-likelihood: " << likelihood << endl;
	cout << "Normalized distances: " << affinity << endl << endl;
	
	// Clean up.
	for (int i = 0; i < files.size(); i++) {
		delete sounds[i];
		delete comparators[i];
		delete feature_sets[i];
		delete loudness[i];
		delete spectral_sparsity[i];
		delete spectral_centroid[i];
		delete transient_index[i];
		delete harmonicity[i];
	}
	
	return 0;
}
