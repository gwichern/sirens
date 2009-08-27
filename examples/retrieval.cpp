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
	sound->open("sound.wav");
	
	// Initialize features.	
	Loudness* loudness = new Loudness(sound->getFrameCount() - 1);
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
	
	FeatureSet* features = new FeatureSet();
	features->addSampleFeature(loudness);
		
	// Extract features.	
	sound->setFeatures(features);
	sound->extractFeatures();	
	sound->close();
	
	// Segment sound.
	RetrievalModel* loudness_comparator = new RetrievalModel(loudness);
	loudness_comparator->initialize();
	write_csv_file("smoothed_loudness.csv", *(loudness_comparator->getTrajectory()));
	
	// Clean up.
	delete sound;
	delete features;
	delete loudness_comparator;
	delete loudness;
	
	return 0;
}
