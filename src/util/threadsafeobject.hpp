#pragma once

#include "threadsafe.hpp"

namespace StardustXRServer {

template <class T>
class ThreadSafeObject : public ThreadSafe<T> {
public:
	ThreadSafeObject() {}
	~ThreadSafeObject() {}

	void set(T value) {
		this->tryLock();
		object = value;
	}

	T &value() {
		this->tryLock();
		return object;
	}

protected:
	T object;
};

} // namespace StardustXRServer
