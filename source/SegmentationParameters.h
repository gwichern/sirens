#ifndef __FEATURESEGMENTER_H__
#define __FEATURESEGMENTER_H__

#include <vector>
using namespace std;

namespace Sirens {
	class SegmentationParameters {
	protected:
		bool initialized;
		
		// Feature normalization.
		double minFeatureValue;
		double maxFeatureValue;
	
		// Each feature lags behind the primary segemntation mode.
		double pLagPlus;
		double pLagMinus;
	
		// Low-pass filter on feature value.
		double alpha;
		
		// Kalman filter parameters.
		double r;
		
		// Prior probabilities for features' modes--initialize xInit/pInit.
		double cStayOff;
		double cStayOn;
		double cTurnOn;
		double cTurningOn;
		double cTurnOff;
		double cNewSegment;
				
		void createQTable();		
		void createFusionLogic();
		
	public:
		SegmentationParameters();
		~SegmentationParameters();
		
		// Attributes.
		void setMinFeatureValue(double value);
		void setMaxFeatureValue(double value);
		double getMinFeatureValue();
		double getMaxFeatureValue();
	
		void setPLagPlus(double value);
		void setPLagMinus(double value);
		double getPLagPlus();
		double getPLagMinus();
	
		void setAlpha(double value);
		double getAlpha();
		
		void setR(double value);
		void setCStayOff(double value);
		void setCStayOn(double value);
		void setCTurnOn(double value);
		void setCTurningOn(double value);
		void setCTurnOff(double value);
		void setCNewSegment(double value);
		double getR();
		double getCStayOff();
		double getCStayOn();
		double getCTurnOn();
		double getCTurningOn();
		double getCTurnOff();
		double getCNewSegment();
		
		double xInit[2];
		double pInit[2][2];
		double q[3][3];
		double fusionLogic[3][3][3][3];
		
		// Operations.
		void initialize();
	};
}

#endif