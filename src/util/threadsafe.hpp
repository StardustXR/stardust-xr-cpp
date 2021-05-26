#pragma once

#include <pthread.h>
#include <functional>
#include <stddef.h>
#include <inttypes.h>
#include <vector>

namespace StardustXRServer {

template <class T>
class ThreadSafe {
public:
	ThreadSafe() {
		pthread_mutex_init(&lockMutex, nullptr);
	}
	~ThreadSafe() {
		pthread_mutex_destroy(&lockMutex);
	}

	void done() {
		pthread_t thisThread = pthread_self();
		int lockedPosition = lockPosition(thisThread);
		if(lockedPosition >= 0) {
			lockedThreads.erase(lockedThreads.begin()+lockedPosition);
			pthread_mutex_unlock(&lockMutex);
		}
	}

protected:
	pthread_mutex_t lockMutex;
	std::vector<pthread_t> lockedThreads;
	bool isLocked(pthread_t &currentThread) {
		return lockPosition(currentThread) != -1;
	}
	int lockPosition(pthread_t &currentThread) {
		for(uint32_t i=0; i<lockedThreads.size(); ++i) {
			if(pthread_equal(lockedThreads[i], currentThread)) {
				return i;
			}
		}
		return -1;
	}
	void tryLock() {
		pthread_t thisThread = pthread_self();
		if(!isLocked(thisThread)) {
			lockedThreads.push_back(thisThread);
			pthread_mutex_lock(&lockMutex);
		}
	}
};

} // namespace StardustXRServer
