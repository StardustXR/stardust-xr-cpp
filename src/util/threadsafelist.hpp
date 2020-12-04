#ifndef STARDUSTXRSERVER_THREADSAFELIST_HPP
#define STARDUSTXRSERVER_THREADSAFELIST_HPP

#include <pthread.h>
#include <functional>
#include <stddef.h>
#include <inttypes.h>
#include <vector>

namespace StardustXRServer {

template <class T>
class ThreadSafeList {
public:
	ThreadSafeList();
	~ThreadSafeList();

	uint32_t length();
	bool exists(int index);

	T &operator[](int i);
	T &at(int i);

	typedef std::function<void(uint32_t, T &)> ForEachFunction;
	void forEach(ForEachFunction function);

	void pushFront(const T &object);
	void pushBack(const T &object);
	void erase(int index);

	void done();

protected:
	pthread_mutex_t lockMutex;
	std::vector<pthread_t> lockedThreads;
	void tryLock();
	bool isLocked(pthread_t &currentThread);
	int lockPosition(pthread_t &currentThread);

	struct ListItem {
		struct ListItem *previous;
		T *value;
		struct ListItem *next;
	};
	typedef struct ListItem ListItem;

	ListItem *begin = nullptr;
	ListItem *end = nullptr;

	uint32_t listLength;
	bool lengthDirty;
	ListItem *get(int index);
};

} // namespace StardustXRServer

#include "threadsafelist_impl.hpp"

#endif // STARDUSTXRSERVER_THREADSAFELIST_HPP
