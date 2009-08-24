#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

// Simple thread wrapper to avoid pthread calls.
class Thread {
protected:
	pthread_t thread;
	
public:	
	Thread();
	
	bool start(void* (*routine)(void*), void* data = NULL);
	bool cancel();
	bool wait();
};

#endif
