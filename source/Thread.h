#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

class Thread {
protected:
	pthread_t thread;
	
public:
	virtual void run() = 0;
	void start();
};

void *start_thread(void* data);

#endif
