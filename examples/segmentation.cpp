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
	loudness->getSegmentationParameters()->setR(0.00088);
	loudness->getSegmentationParameters()->setCStayOff(0.0015);
	loudness->getSegmentationParameters()->setCTurnOn(0.085);
	loudness->getSegmentationParameters()->setCTurningOn(0.085);
	loudness->getSegmentationParameters()->setCTurnOff(0.085);
	loudness->getSegmentationParameters()->setCNewSegment(0.085);
	loudness->getSegmentationParameters()->setCStayOn(0.015);
	loudness->getSegmentationParameters()->setPLagPlus(0.001);
	loudness->getSegmentationParameters()->setPLagMinus(0.9);
	
	TemporalSparsity* temporal_sparsity = new TemporalSparsity(sound->getFrameCount());
	temporal_sparsity->setMin(1.0 / 50.0);
	temporal_sparsity->setMax(0.5247);
	temporal_sparsity->getSegmentationParameters()->setAlpha(0.05);
	temporal_sparsity->getSegmentationParameters()->setR(0.00021);
	temporal_sparsity->getSegmentationParameters()->setCStayOff(0.000033506);
	temporal_sparsity->getSegmentationParameters()->setCTurnOn(0.0093);
	temporal_sparsity->getSegmentationParameters()->setCTurningOn(0.0093);
	temporal_sparsity->getSegmentationParameters()->setCTurnOff(0.0093);
	temporal_sparsity->getSegmentationParameters()->setCNewSegment(0.0093);
	temporal_sparsity->getSegmentationParameters()->setCStayOn(0.00196018);
	temporal_sparsity->getSegmentationParameters()->setPLagPlus(0.001);
	temporal_sparsity->getSegmentationParameters()->setPLagMinus(0.01);
	
	SpectralSparsity* spectral_sparsity = new SpectralSparsity(sound->getFrameCount());
	spectral_sparsity->setMin(1.0 / 1024.0);
	spectral_sparsity->setMax(0.6509);
	spectral_sparsity->getSegmentationParameters()->setAlpha(0.05);
	spectral_sparsity->getSegmentationParameters()->setR(0.0071);
	spectral_sparsity->getSegmentationParameters()->setCStayOff(0.000033506);
	spectral_sparsity->getSegmentationParameters()->setCTurnOn(0.0093);
	spectral_sparsity->getSegmentationParameters()->setCTurningOn(0.0093);
	spectral_sparsity->getSegmentationParameters()->setCTurnOff(0.0093);
	spectral_sparsity->getSegmentationParameters()->setCNewSegment(0.0093);
	spectral_sparsity->getSegmentationParameters()->setCStayOn(0.00196018);
	spectral_sparsity->getSegmentationParameters()->setPLagPlus(0.001);
	spectral_sparsity->getSegmentationParameters()->setPLagMinus(0.01);
	
	SpectralCentroid* spectral_centroid = new SpectralCentroid(sound->getFrameCount(), sound->getSpectrumSize(), sound->getSampleRate());
	spectral_centroid->setMin(0.4994);
	spectral_centroid->setMax(25.7848);
	spectral_centroid->getSegmentationParameters()->setAlpha(0.05);
	spectral_centroid->getSegmentationParameters()->setR(0.00051);
	spectral_centroid->getSegmentationParameters()->setCStayOff(0.000033506);
	spectral_centroid->getSegmentationParameters()->setCTurnOn(0.0093);
	spectral_centroid->getSegmentationParameters()->setCTurningOn(0.0093);
	spectral_centroid->getSegmentationParameters()->setCTurnOff(0.0093);
	spectral_centroid->getSegmentationParameters()->setCNewSegment(0.0093);
	spectral_centroid->getSegmentationParameters()->setCStayOn(0.00096018);
	spectral_centroid->getSegmentationParameters()->setPLagPlus(0.001);
	spectral_centroid->getSegmentationParameters()->setPLagMinus(0.01);

	TransientIndex* transient_index = new TransientIndex(sound->getFrameCount());
	spectral_sparsity->setMin(0.0197);
	spectral_sparsity->setMax(42.5258);
	spectral_sparsity->getSegmentationParameters()->setAlpha(0.05);
	spectral_sparsity->getSegmentationParameters()->setR(0.00051);
	spectral_sparsity->getSegmentationParameters()->setCStayOff(0.000033506);
	spectral_sparsity->getSegmentationParameters()->setCTurnOn(0.0093);
	spectral_sparsity->getSegmentationParameters()->setCTurningOn(0.0093);
	spectral_sparsity->getSegmentationParameters()->setCTurnOff(0.0093);
	spectral_sparsity->getSegmentationParameters()->setCNewSegment(0.0093);
	spectral_sparsity->getSegmentationParameters()->setCStayOn(0.00096018);
	spectral_sparsity->getSegmentationParameters()->setPLagPlus(0.001);
	spectral_sparsity->getSegmentationParameters()->setPLagMinus(0.01);
		
	FeatureSet* features = new FeatureSet();
	features->addSampleFeature(loudness);
	features->addSampleFeature(temporal_sparsity);
	//features->addSpectralFeature(spectral_sparsity);
	//features->addSpectralFeature(spectral_centroid);
	
	// Extract features.	
	sound->setFeatures(features);
	sound->extractFeatures();	
	sound->close();
	
	// Segment sound.
	Segmenter* segmenter = new Segmenter(0.001, 0.005, 20);
	vector<vector<double> > segments = segmenter->getSegments(features);
	
	// Output segments.
	for (int i = 0; i < segments.size(); i++)
		cout << i << ": " << segments[i][0] << "-" << segments[i][1] << endl;
	
	// Clean up.
	delete sound;
	delete segmenter;
	delete features;
	delete loudness;
	delete spectral_sparsity;
	delete spectral_centroid;
	
	return 0;
}
