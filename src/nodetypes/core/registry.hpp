#pragma once

#include "typednoderef.hpp"

#include <mutex>
#include <vector>

namespace StardustXRServer {

template<class T>
class Registry {
	std::mutex mutex;
	std::vector<TypedNodeRef<T>> items;

public:
	void add(T *node) {
		std::unique_lock<std::mutex> lock(mutex);
		items.emplace_back(node);
	}
	void remove(T *node) {
		std::unique_lock<std::mutex> lock(mutex);
		if(items.size() > 0)
			items.erase(std::remove(items.begin(), items.end(), node));
	}
	std::vector<T *> list() {
		std::vector<T *> nodes;
		std::unique_lock<std::mutex> lock(mutex);
		for(TypedNodeRef<T> &ref : items) {
			if(ref)
				nodes.push_back(ref.ptr());
		}
		return nodes;
	}
};

}
