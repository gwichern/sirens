/*
	Copyright 2007, 2008, 2009 Brandon Mechtley
	
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

#ifndef FEATURE_H
#define FEATURE_H

#include <pthread.h>
#include <string>
using namespace std;

#include "Thread.h"
#include "CircularArray.h"
#include "segmentation/SegmentationParameters.h"

namespace Sirens {
	class Feature {
	private:
		Thread thread;
		
		// Segmentation.
		SegmentationParameters* segmentationParameters;
		
		// History of every feature value calculated.
		CircularArray* history;
		
		// Normalization. Values are scaled between min and max.
		double min, max;
		
		virtual void initialize() {};				// on-demand initialization, as some parameters may not be set on construction.
		virtual void freeMemory() {};				// called on re-initialization, to free up any memory allocated at initialization.
	
	protected:
		CircularArray* input;						// input buffer to perform calculations on.
		bool initialized;							// whether or not the feature has been initialized.
		double value;								// actual value calculated.
			
	public:
		Feature(int history_size = 1);
		virtual ~Feature();
		
		// Calculation.
		void calculate(CircularArray* input_in);
		void prepareCalculation();
		virtual void performCalculation() {}		// actual implementation.
		void waitForCompletion();					// blocks until the calculation is complete.
		double getValue();
		
		// Access to feature value history.
		void addHistoryFrame(double value);			// Adds a fake history frame--useful for loading from files etc.
		CircularArray* getHistory();
		double getHistoryFrame(int i);
		int getHistorySize();
		int getMaxHistorySize();
		void setMaxHistorySize(int history_size);
		
		// Normalization.
		void setMax(double value);
		void setMin(double value);
		double getMax();
		double getMin();
		
		// Segmentation.
		SegmentationParameters* getSegmentationParameters();
		
		// String representation.
		virtual string toString();
		string historyString();	
	};
	
	static void* run_feature(void* data) {
		Feature* feature = (Feature*)data;

		feature->prepareCalculation();
	}
}

#endif
