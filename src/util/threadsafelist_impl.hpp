#include "threadsafelist.hpp"
#include <cmath>

namespace StardustXRServer {

template<class T>
ThreadSafeList<T>::ThreadSafeList() {
	pthread_mutex_init(&lockMutex, nullptr);
}

template<class T>
ThreadSafeList<T>::~ThreadSafeList() {
	pthread_mutex_destroy(&lockMutex);
}

template<class T>
uint32_t ThreadSafeList<T>::length() {
	tryLock();
	if(!lengthDirty)
		return listLength;

	listLength = (bool) begin;
	ListItem *currentItem = begin;

	for(; currentItem->next != nullptr; listLength++)
		currentItem = currentItem->next;

	return listLength;
}

template<class T>
void ThreadSafeList<T>::pushFront(const T &object) {
	tryLock();
	lengthDirty = true;
	ListItem *newItem = new ListItem();
	newItem->value = new T(object);
	newItem->next = begin;
	begin = newItem;
	if(end == nullptr)
		end = begin;
}

template<class T>
void ThreadSafeList<T>::pushBack(const T &object) {
	tryLock();
	lengthDirty = true;
	ListItem *newItem = new ListItem();
	newItem->value = new T(object);
	newItem->previous = end;
	end = newItem;
	if(begin == nullptr)
		begin = end;
}

template<class T>
void ThreadSafeList<T>::erase(int index) {
	tryLock();
	lengthDirty = true;
	ListItem *item = get(index);

	if(item == begin)
		begin = item->next;
	else
		item->previous->next = item->next;

	if(item == end)
		end = item->previous;
	else
		item->next->previous = item->previous;

	delete item;
}

template<class T>
bool ThreadSafeList<T>::exists(int index) {
	return (index >= 0) ? index+1 : -index > length();
}

template<class T>
T &ThreadSafeList<T>::at(int i) {
	tryLock();
	return *get(i).value;
}

template<class T>
void ThreadSafeList<T>::forEach(ForEachFunction function) {
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

template<class T>
T &ThreadSafeList<T>::operator[](int i) {
	return at(i);
}

template<class T>
void ThreadSafeList<T>::done() {
	if(locked) {
		locked = false;
		pthread_mutex_unlock(&lockMutex);
	}
}

template<class T>
void ThreadSafeList<T>::tryLock() {
	if(!locked) {
		locked = true;
		pthread_mutex_lock(&lockMutex);
	}
}

template<class T>
typename ThreadSafeList<T>::ListItem *ThreadSafeList<T>::get(int index) {
	tryLock();

	int iMax = (index >= 0) ? index : length() + index;
	ListItem *currentItem = begin;

	for(int i=1; i<iMax; ++i)
		currentItem = currentItem->next;

	return currentItem;
}

} // namespace StardustXRServer
