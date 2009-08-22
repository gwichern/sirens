#include "CircularArray.h"

#include "support/math_support.h"
#include "support/string_support.h"

namespace Sirens {
	CircularArray::CircularArray(int max_size, bool pad_next_power) {
		start = 0;
		size = 0;
		index = 0;
		maxSize = max_size;
		
		if (!pad_next_power)
			allocatedSize = maxSize;
		else
			allocatedSize = next_pow(maxSize, 2);
		
		data = new double[allocatedSize];
		
		for (int i = 0; i < allocatedSize; i++)
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

	double* CircularArray::getData() {
		return data;
	}
	
	int CircularArray::getSize() {
		return size;
	}
	
	int CircularArray::getMaxSize() {
		return maxSize;
	}
	
	int CircularArray::getStart() {
		return start;
	}
	
	int CircularArray::getIndex() {
		return index;
	}
	
	double CircularArray::getValueAt(int offset) {
		return data[(start + offset) % maxSize];
	}
	
	string CircularArray::toString() {
		string history_string = 
			"Allocated size: " + double_to_string(allocatedSize) + 
			", Max size: " + double_to_string(maxSize) + 
			", Size: " + double_to_string(size) + 
			", Start: " + double_to_string(start) + "\n\t";
		
		for (int i = 0; i < size; i++)
			history_string += double_to_string(data[i]) + " ";
		
		return history_string;
	}
}
