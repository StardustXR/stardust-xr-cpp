#pragma once

#include <pthread.h>
#include <functional>
#include <stddef.h>
#include <inttypes.h>
#include <vector>

namespace StardustXRServer {

template <class T>
class ThreadSafeList {
public:
	ThreadSafeList() {
		pthread_mutex_init(&lockMutex, nullptr);
	}
	~ThreadSafeList() {
		pthread_mutex_destroy(&lockMutex);
	}

	uint32_t length() {
		tryLock();
		if(!lengthDirty)
			return listLength;

		listLength = (bool) begin;
		ListItem *currentItem = begin;

		for(; currentItem && currentItem->next != nullptr; listLength++)
			currentItem = currentItem->next;

		return listLength;
	}
	bool exists(int index) {
		return (index >= 0) ? index+1 : -index > length();
	}

	T &operator[](int i) {
		return at(i);
	}
	T &at(int i) {
		tryLock();
		return *get(i)->value;
	}

	typedef std::function<void(uint32_t, T &)> ForEachFunction;
	void forEach(ForEachFunction function) {
		tryLock();

		uint32_t i = 0;
		ListItem *currentItem = begin;
		while(currentItem) {
			ListItem *nextItem = currentItem->next;
			function(i, *currentItem->value);
			currentItem = nextItem;
			i++;
		}
	}

	void pushFront(const T &object) {
		tryLock();
		lengthDirty = true;
		ListItem *newItem = new ListItem();
		newItem->value = new T(object);
		newItem->next = begin;
		if(begin)
			begin->previous = newItem;
		begin = newItem;
		if(end == nullptr)
			end = begin;
	}
	void pushBack(const T &object) {
		tryLock();
		lengthDirty = true;
		ListItem *newItem = new ListItem();
		newItem->value = new T(object);
		newItem->previous = end;
		if(end)
			end->next = newItem;
		end = newItem;
		if(begin == nullptr)
			begin = end;
	}
	void erase(int index) {
		tryLock();
		lengthDirty = true;
		ListItem *item = get(index);

		if(item == begin)
			begin = item->next;
		else if(item->previous)
			item->previous->next = item->next;

		if(item == end)
			end = item->previous;
		else if(item->next)
			item->next->previous = item->previous;

		delete item->value;
		delete item;
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
	void tryLock() {
		pthread_t thisThread = pthread_self();
		if(!isLocked(thisThread)) {
			lockedThreads.push_back(thisThread);
			pthread_mutex_lock(&lockMutex);
		}
	}
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
	ListItem *get(int index) {
		tryLock();

		int iMax = (index >= 0) ? index : length() + index;
		ListItem *currentItem = begin;

		for(int i=0; i<iMax; ++i)
			currentItem = currentItem->next;

		return currentItem;
	}
};

} // namespace StardustXRServer
