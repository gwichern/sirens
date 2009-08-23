#include "CircularArray.h"

#include "support/math_support.h"
#include "support/string_support.h"

namespace Sirens {
	CircularArray::CircularArray(int max_size) {
		start = 0;
		size = 0;
		index = 0;
		maxSize = max_size;
		 
		data = new double[maxSize];
		
		for (int i = 0; i < maxSize; i++)
			data[i] = 0;
	}

	CircularArray::~CircularArray() {
		delete [] data;
	}
	
	void CircularArray::addValue(double value) {	
		data[index] = value;
		
		if (size == maxSize)
			start = (start + 1) % maxSize;
		else
			size ++;
		
		index = (index + 1) % maxSize;
	}
	
	int CircularArray::getSize() {
		int return_value = -1;
		
		return_value = size;
		
		return return_value;
	}
	
	int CircularArray::getMaxSize() {
		int return_value = -1;
		
		return_value = maxSize;
		
		return return_value;
	}
	
	int CircularArray::getStart() {
		int return_value = -1;
		
		return_value = start;
		
		return return_value;
	}
	
	int CircularArray::getIndex() {
		int return_value = -1;
		
		return_value = index;
		
		return return_value;
	}
	
	double CircularArray::getValue(int offset) {
		double return_value = -1;
		
		return_value = data[(start + offset) % maxSize];
		
		return return_value;
	}
	
	string CircularArray::toString() {
		string history_string;
				
		history_string = 
			"Max size: " + double_to_string(maxSize) + 
			", Size: " + double_to_string(size) + 
			", Start: " + double_to_string(start) + "\n\t";
		
		for (int i = 0; i < size; i++)
			history_string += double_to_string(data[i]) + " ";
				
		return history_string;
	}
}
