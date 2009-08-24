#include "Thread.h"

Thread::Thread() {
	thread = 0;
};

bool Thread::start(void* (*routine)(void*), void* data) {
	return (!thread && pthread_create(&thread, NULL, *routine, data) == 0);
}

bool Thread::cancel() {
	return (pthread_cancel(thread) == 0);
}

bool Thread::wait() {
	if (pthread_join(thread, NULL) == 0) {
		thread = 0;
		return true;
	} else
		return false;
}
