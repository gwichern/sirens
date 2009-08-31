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
#include "../source/support/string_support.h"
using namespace Sirens;

int main() {
	// Open sound.
	Sound* sound = new Sound();
	sound->setFrameLength(0.04);
	sound->setHopLength(0.02);
	sound->open("sound/whistle4.wav");
	
	cout << "sound.wav" << endl;
	cout << "\tSamples: " << sound->getSampleCount() << endl;
	cout << "\tFrames: " << sound->getFrameCount() << endl;
	cout << "\tSpectrum size: " << sound->getSpectrumSize() << endl;
	cout << "\tSample rate: " << sound->getSampleRate() << endl;
	
	// Initialize features.	
	Loudness* loudness = new Loudness(sound->getFrameCount());
	loudness->setMin(-60);
	loudness->setMax(0);
	loudness->getSegmentationParameters()->setAlpha(0.15);
	loudness->getSegmentationParameters()->setR(0.0098);
	loudness->getSegmentationParameters()->setCStayOff(0.0015);
	loudness->getSegmentationParameters()->setCTurnOn(0.085);
	loudness->getSegmentationParameters()->setCTurningOn(0.085);
	loudness->getSegmentationParameters()->setCTurnOff(0.085);
	loudness->getSegmentationParameters()->setCNewSegment(0.085);
	loudness->getSegmentationParameters()->setCStayOn(0.05);
	loudness->getSegmentationParameters()->setPLagPlus(0.75);
	loudness->getSegmentationParameters()->setPLagMinus(0.75);
	
	SpectralCentroid* spectral_centroid = new SpectralCentroid(sound->getFrameCount(), sound->getSpectrumSize(), sound->getSampleRate());
	spectral_centroid->setMin(0.4994);
	spectral_centroid->setMax(25.7848);
	spectral_centroid->getSegmentationParameters()->setAlpha(0.05);
	spectral_centroid->getSegmentationParameters()->setR(0.00000196);
	spectral_centroid->getSegmentationParameters()->setCStayOff(0.0000933506);
	spectral_centroid->getSegmentationParameters()->setCTurnOn(0.85);
	spectral_centroid->getSegmentationParameters()->setCTurningOn(0.85);
	spectral_centroid->getSegmentationParameters()->setCTurnOff(0.85);
	spectral_centroid->getSegmentationParameters()->setCNewSegment(0.85);
	spectral_centroid->getSegmentationParameters()->setCStayOn(0.0025296018);
	spectral_centroid->getSegmentationParameters()->setPLagPlus(0.75);
	spectral_centroid->getSegmentationParameters()->setPLagMinus(0.075);
	
	SpectralSparsity* spectral_sparsity = new SpectralSparsity(sound->getFrameCount());
	spectral_sparsity->setMin(0);
	spectral_sparsity->setMax(0.6509);
	spectral_sparsity->getSegmentationParameters()->setAlpha(0.05);
	spectral_sparsity->getSegmentationParameters()->setR(0.0196);
	spectral_sparsity->getSegmentationParameters()->setCStayOff(0.001833506);
	spectral_sparsity->getSegmentationParameters()->setCTurnOn(0.85);
	spectral_sparsity->getSegmentationParameters()->setCTurningOn(0.85);
	spectral_sparsity->getSegmentationParameters()->setCTurnOff(0.85);
	spectral_sparsity->getSegmentationParameters()->setCNewSegment(0.85);
	spectral_sparsity->getSegmentationParameters()->setCStayOn(.009296018);
	spectral_sparsity->getSegmentationParameters()->setPLagPlus(0.75);
	spectral_sparsity->getSegmentationParameters()->setPLagMinus(0.75);
	
	FeatureSet* features = new FeatureSet();
	features->addSampleFeature(loudness);
	features->addSpectralFeature(spectral_centroid);
	features->addSpectralFeature(spectral_sparsity);
	
	// Extract features.	
	sound->setFeatures(features);
	sound->extractFeatures();	
	sound->close();
	
	cout << "Extraction complete." << endl;
	
	// Segment sound.
	Segmenter* segmenter = new Segmenter(0.00000000001, 0.00000000001);
	segmenter->segment(features);
	
	cout << "Segmentation complete." << endl;
	
	vector<int> modes = segmenter->getModes();
	
	vector<double> modes_double(modes.size());
	
	for (int i = 0; i < modes.size(); i++)
		modes_double[i] = modes[i];
		
	write_csv_file("modes.csv", vector<double>(modes_double));
	
	vector<vector<double> > segments = segmenter->getSegments();
	
	// Output segments.
	cout << "\tSegments: " << endl;
	
	for (int i = 0; i < segments.size(); i++)
		cout << "\t\t" << i << ": " << segments[i][0] << "-" << segments[i][1] << endl;
	
	// Clean up.
	delete sound;
	delete segmenter;
	delete features;
	delete loudness;
	delete spectral_centroid;
	delete spectral_sparsity;
	
	return 0;
}
