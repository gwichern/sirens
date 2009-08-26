/*
	Copyright 2008, 2009 Brandon Mechtley

	This file is part of Sirens.

	Sirens is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Sirens is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Sirens.  If not, see <http://www.gnu.org/licenses/>.
*/

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
