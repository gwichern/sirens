#include "SegmentationParameters.h"

namespace Sirens {
	SegmentationParameters::SegmentationParameters() {
		xInit[0] = 0;
		xInit[1] = 0;
		
		pInit[0][0] = 1;
		pInit[0][1] = 0;
		pInit[1][0] = 0;
		pInit[1][1] = 1;
		
		initialized = false;
	}
	
	SegmentationParameters::~SegmentationParameters() {
	}

	void SegmentationParameters::setPLagPlus(double value) {
		pLagPlus = value;
	}

	void SegmentationParameters::setPLagMinus(double value) {
		pLagMinus = value;
	}

	double SegmentationParameters::getPLagPlus() {
		return pLagPlus;
	}

	double SegmentationParameters::getPLagMinus() {
		return pLagMinus;
	}

	void SegmentationParameters::setAlpha(double value) {
		alpha = value;
	}

	double SegmentationParameters::getAlpha() {
		return alpha;
	}
	
	void SegmentationParameters::setR(double value) {
		r = value;
	}
	
	void SegmentationParameters::setCStayOff(double value) {
		cStayOff = value;
	}
	
	void SegmentationParameters::setCStayOn(double value) {
		cStayOn = value;
	}
	void SegmentationParameters::setCTurnOn(double value) {
		cTurnOn = value;
	}
	
	void SegmentationParameters::setCTurningOn(double value) {
		cTurningOn = value;
	}
	
	void SegmentationParameters::setCTurnOff(double value) {
		cTurnOff = value;
	}
	
	void SegmentationParameters::setCNewSegment(double value) {
		cNewSegment = value;
	}
	
	double SegmentationParameters::getR() {
		return r;
	}
	
	double SegmentationParameters::getCStayOff() {
		return cStayOff;
	}
	
	double SegmentationParameters::getCStayOn() {
		return cStayOn;
	}
	
	double SegmentationParameters::getCTurnOn() {
		return cTurnOn;
	}
	
	double SegmentationParameters::getCTurningOn() {
		return cTurningOn;
	}
	
	double SegmentationParameters::getCTurnOff() {
		return cTurnOff;
	}
	
	double SegmentationParameters::getCNewSegment() {
		return cNewSegment;
	}
	
	void SegmentationParameters::createFusionLogic() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					for (int l = 0; l < 3; l++)
						fusionLogic[i][j][k][l] = 0;
				}
			}
		}	
		
		fusionLogic[0][0][0][0] = 1;
	
		fusionLogic[0][0][1][0] = 1 - pLagMinus;
		fusionLogic[0][0][1][2] = pLagMinus;

		fusionLogic[0][0][2][0] = 1 - pLagMinus;
		fusionLogic[0][0][2][2] = pLagMinus;

		fusionLogic[0][1][0][0] = pLagPlus;
		fusionLogic[0][1][0][1] = 1 - pLagPlus;
	
		fusionLogic[0][1][1][0] = pLagPlus - pLagPlus * pLagMinus;
		fusionLogic[0][1][1][1] = 1 - pLagPlus;
		fusionLogic[0][1][1][2] = pLagPlus * pLagMinus;

		fusionLogic[0][1][2][0] = pLagPlus - pLagPlus * pLagMinus;
		fusionLogic[0][1][2][1] = 1 - pLagPlus;
		fusionLogic[0][1][2][2] = pLagPlus * pLagMinus;

		fusionLogic[1][2][0][0] = pLagPlus;
		fusionLogic[1][2][0][1] = 1 - pLagPlus;

		fusionLogic[1][2][1][2] = 1;

		fusionLogic[1][2][2][0] = pLagPlus - pLagPlus * pLagMinus;
		fusionLogic[1][2][2][1] = 1 - pLagPlus;
		fusionLogic[1][2][2][2] = pLagPlus * pLagMinus;

		fusionLogic[2][0][0][0] = 1;
	
		fusionLogic[2][0][1][0] = 1 - pLagMinus;
		fusionLogic[2][0][1][2] = pLagMinus;

		fusionLogic[2][0][2][0] = 1 - pLagMinus;
		fusionLogic[2][0][2][2] = pLagMinus;

		fusionLogic[2][1][0][0] = pLagPlus;
		fusionLogic[2][1][0][1] = 1 - pLagPlus;

		fusionLogic[2][1][1][0] = pLagPlus - pLagPlus * pLagMinus;
		fusionLogic[2][1][1][1] = 1 - pLagPlus;
		fusionLogic[2][1][1][2] = pLagPlus * pLagMinus;

		fusionLogic[2][1][2][0] = pLagPlus - pLagPlus * pLagMinus;
		fusionLogic[2][1][2][1] = 1 - pLagPlus;
		fusionLogic[2][1][2][2] = pLagPlus * pLagMinus;

		fusionLogic[2][2][0][0] = pLagPlus;
		fusionLogic[2][2][0][1] = 1 - pLagPlus;

		fusionLogic[2][2][1][2] = 1;

		fusionLogic[2][2][2][2] = 1;
	}
	
	void SegmentationParameters::createQTable() {
		q[0][0] = cStayOff;
		q[0][1] = cTurnOn;
		q[0][2] = 0;
		q[1][0] = cTurnOn;
		q[1][1] = cTurnOn;
		q[1][2] = cTurningOn;
		q[2][0] = cTurnOff;
		q[2][1] = cNewSegment;
		q[2][2] = cStayOn;
	}
	
	void SegmentationParameters::initialize() {
		if (!initialized) {
			createFusionLogic();
			createQTable();
			
			initialized = true;
		}
	}
}
