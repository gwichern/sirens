#include "Feature.h"

namespace Sirens {
	Feature::Feature(int history_size) {
		history = new CircularArray(history_size, false);
	}
	
	Feature::~Feature() {
		delete history;
	}
	
	void Feature::calculate(CircularArray* input_in) {
		input = input_in;
	
		thread.start(StartFeatureThread, (void*)this);
		
		history->addValue(value);
	}
	
	void Feature::waitForCompletion() {
		thread.wait();
	}
	
	double Feature::getValue() {
		return value;
	}
	
	CircularArray* Feature::getHistory() {
		return history;
	}
	
	double Feature::getHistoryFrame(int i) {
		return history->getValueAt(i);
	}
	
	void Feature::prepareCalculation() {
		if (!initialized) {
			freeMemory();
			initialize();
			
			initialized = true;
		}
		
		performCalculation();
	}
	
	void Feature::setMaxHistorySize(int history_size) {
		CircularArray* new_history = new CircularArray(history_size, false);
		
		if (history) {
			for (int i = 0; i < history->getSize(); i++)
				history->addValue(history->getValueAt(i));
				
			delete history;
		}
		
		history = new_history;
	}
	
	
	int Feature::getMaxHistorySize() {
		return history->getMaxSize();
	}
	
	int Feature::getHistorySize() {
		return history->getSize();
	}
	
	string Feature::toString() {
		return "Feature";
	}
	
	string Feature::historyString() {
		return toString() + "\n\t" + history->toString();
	}
}
