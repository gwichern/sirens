#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

// Simple thread wrapper to avoid pthread calls.
class Thread {
protected:
	pthread_t thread;
	
public:	
	virtual void run() = 0;				// Implement this--is called upon start(). Usually a nonstop loop of some sort.
	void start();
};

// Static method for beginning threads with pthread.
void *start_thread(void* data);

#endif
