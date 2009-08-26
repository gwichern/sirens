/*
	Copyright 2007, 2009 Brandon Mechtley, Gordon Wichern, Alex Fink, and Harvey Thornburg

	This file is part of Sirens.

	Sirens is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Sirens is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Sirens.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TRANSIENTINDEX_H
#define TRANSIENTINDEX_H

#include "../Feature.h"

namespace Sirens {
	class TransientIndex : public Feature {
	private:
		double* mfccOld;
		double* mfccNew;
		double* dctMatrix;
		double* filterTemp;
		double* filterBank;
		
		int filters, mels, spectrumSize, sampleRate;
		
		void freeMemory();
		void initialize();
		
	public:
		TransientIndex(int history_size = 1, int spectrum_size = 1024, int sample_rate = 44100, int in_filters = 30, int in_mels = 15);
		~TransientIndex();
		
		void setSpectrumSize(int spectrum_size);
		void setSampleRate(int sample_rate);
		void setFilters(int in_filters);
		void setMels(int in_mels);
		
		int getSpectrumSize();
		int getSampleRate();
		int getFilters();
		int getMels();
		
		virtual void performCalculation();
		virtual string toString();
	};
}

#endif
