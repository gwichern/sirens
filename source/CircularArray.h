#ifndef CIRCULARARRAY_H
#define CIRCULARARRAY_H

#include <string>
using namespace std;

#include <pthread.h>

// Circular array allows values to be added and simply replace older values if the maximum size is reached.
namespace Sirens {
	class CircularArray {
	private:		
		double* data;
		
		int size;											// Current size of the array.
		int maxSize;										// Maximum size before wrapping to the front of the array.
		int allocatedSize;									// Total space allocated for the array (>= maxSize), for zero-padding.
		int start;											// Pointer to the start of the array. Not always zero.
		int index;											// Pointer to the last element of the array.
		
	public:
		CircularArray(int max_size);
		~CircularArray();
		
		void addValue(double value);
		
		int getSize();
		int getMaxSize();
		int getStart();
		int getIndex();
		
		double getUnorderedValue(int offset);
		double getValue(int offset);
		
		double* getData() {
			return data;
		}
		
		string toString();
	};
}

#endif
