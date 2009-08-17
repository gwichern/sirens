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
	spectral_sparsity->getSegmentationParameters()->setCStayOn(0.009296018);
	spectral_sparsity->getSegmentationParameters()->setPLagPlus(0.75);
	spectral_sparsity->getSegmentationParameters()->setPLagMinus(0.075);
	
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
	spectral_centroid->getSegmentationParameters()->setPLagMinus(0.75);
	
	FeatureSet* features = new FeatureSet();
	features->addSampleFeature(loudness);
	features->addSpectralFeature(spectral_sparsity);
	features->addSpectralFeature(spectral_centroid);
	
	// Extract features.	
	sound->setFeatures(features);
	sound->extractFeatures();	
	sound->close();
	
	// Segment sound.
	Segmenter* segmenter = new Segmenter(0.00000000001, 0.00000000001, 20);
	vector<vector<double> > segments = segmenter->getSegments(features);
	
	// Output segments.
	for (int i = 0; i < segments.size(); i++)
		cout << i << ": " << segments[i][0] << "-" << segments[i][0] + segments[i][1] << endl;
	
	// Clean up.
	delete sound;
	delete segmenter;
	delete features;
	delete loudness;
	delete spectral_sparsity;
	delete spectral_centroid;
	
	return 0;
}
