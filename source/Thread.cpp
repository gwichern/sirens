#include "Thread.h"

void Thread::start() {
	pthread_create(&thread, NULL, start_thread, (void*)this);
}

void *start_thread(void* data) {
	((Thread*)data)->run();
	
	return NULL;
}
