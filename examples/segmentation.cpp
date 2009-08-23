#include <iostream>
using namespace std;

#include "../source/Sirens.h"
using namespace Sirens;

int main() {
	// Open sound.
	Sound* sound = new Sound();
	sound->setFrameLength(0.04);
	sound->setHopLength(0.02);
	sound->open("/Users/brandon/Documents/git/sirens/examples/sound.wav");
	
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
	
	TemporalSparsity* temporal_sparsity = new TemporalSparsity(sound->getFrameCount());
	temporal_sparsity->setMin(1.0 / 50.0);
	temporal_sparsity->setMax(0.5247);
	temporal_sparsity->getSegmentationParameters()->setAlpha(0.05);
	temporal_sparsity->getSegmentationParameters()->setR(0.0196);
	temporal_sparsity->getSegmentationParameters()->setCStayOff(0.001833506);
	temporal_sparsity->getSegmentationParameters()->setCTurnOn(0.85);
	temporal_sparsity->getSegmentationParameters()->setCTurningOn(0.85);
	temporal_sparsity->getSegmentationParameters()->setCTurnOff(0.85);
	temporal_sparsity->getSegmentationParameters()->setCNewSegment(0.85);
	temporal_sparsity->getSegmentationParameters()->setCStayOn(0.009296018);
	temporal_sparsity->getSegmentationParameters()->setPLagPlus(0.75);
	temporal_sparsity->getSegmentationParameters()->setPLagMinus(0.075);
	
	SpectralSparsity* spectral_sparsity = new SpectralSparsity(sound->getFrameCount());
	spectral_sparsity->setMin(1.0 / 1024.0);
	spectral_sparsity->setMax(0.6509);
	spectral_sparsity->getSegmentationParameters()->setAlpha(0.05);
	spectral_sparsity->getSegmentationParameters()->setR(0.0196);
	spectral_sparsity->getSegmentationParameters()->setCStayOff(0.00000833506);
	spectral_sparsity->getSegmentationParameters()->setCTurnOn(0.85);
	spectral_sparsity->getSegmentationParameters()->setCTurningOn(0.85);
	spectral_sparsity->getSegmentationParameters()->setCTurnOff(0.85);
	spectral_sparsity->getSegmentationParameters()->setCNewSegment(0.85);
	spectral_sparsity->getSegmentationParameters()->setCStayOn(0.0019296018);
	spectral_sparsity->getSegmentationParameters()->setPLagPlus(0.75);
	spectral_sparsity->getSegmentationParameters()->setPLagMinus(0.75);
	
	FeatureSet* features = new FeatureSet();
	features->addSampleFeature(loudness);
	features->addSampleFeature(temporal_sparsity);
	features->addSpectralFeature(spectral_sparsity);
	
	// Extract features.	
	sound->setFeatures(features);
	sound->extractFeatures();	
	sound->close();
	
	// Segment sound.
	Segmenter* segmenter = new Segmenter(0.00000000001, 0.00000000001, 20);
	vector<vector<double> > segments = segmenter->getSegments(features);
	
	// Output segments.
	for (int i = 0; i < segments.size(); i++)
		cout << i << ": " << segments[i][0] << "-" << segments[i][1] << endl;
	
	// Clean up.
	delete sound;
	delete segmenter;
	delete features;
	delete loudness;
	delete temporal_sparsity;
	delete spectral_sparsity;
	
	return 0;
}
