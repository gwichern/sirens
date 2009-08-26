/*
	Copyright 2009 Brandon Mechtley
	
	This file is part of Sirens.
	
	Sirens is free software: you can redistribute it and/or modify it under the terms 
	of the GNU Lesser General Public License as  published by the Free Software 
	Foundation, either version 3 of the License, or (at your option) any later version.
	
	Sirens is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
	PURPOSE.  See the GNU General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License along
	with Sirens. If not, see <http://www.gnu.org/licenses/>.
*/

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
