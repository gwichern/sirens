#ifndef CIRCULARARRAY_H
#define CIRCULARARRAY_H

#include <string>
using namespace std;

namespace Sirens {
	class CircularArray {
	private:
		double* data;
		int size, maxSize, allocatedSize, start, index;
		
	public:
		CircularArray(int max_size, bool pad_next_power = false);
		~CircularArray();
		
		void addValue(double value);
		
		double* getData();
		int getSize();
		int getMaxSize();
		int getStart();
		int getIndex();
		
		double getValueAt(int offset);
		
		string toString();
	};
}

#endif
