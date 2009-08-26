/*
	Copyright 2007, 2009 Brandon Mechtley, Gordon Wichern, Alex Fink, and Harvey Thornburg
	
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

#ifndef SPECTRALCENTROID_H
#define SPECTRALCENTROID_H

#include "../Feature.h"

namespace Sirens {
	class SpectralCentroid : public Feature {
	private:		
		double* barkWeights;
		double* barkUnits;
		
		int spectrumSize, sampleRate;
		
		void freeMemory();
		void initialize();
		
	public:
		SpectralCentroid(int history_size = 1, int spectrum_size = 1024, int sample_rate = 44100);
		~SpectralCentroid();
		
		void setSpectrumSize(int spectrum_size);
		void setSampleRate(int sample_rate);
		
		int getSpectrumSize();
		int getSampleRate();
		
		virtual void performCalculation();
		virtual string toString();
	};
}

#endif
