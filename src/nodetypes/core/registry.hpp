#pragma once

#include "node.hpp"
#include "typednoderef.hpp"

#include <mutex>
#include <vector>

namespace StardustXRServer {

template<class T>
class Registry {
	std::mutex mutex;
	std::vector<TypedNodeRef<T>> items;

public:
	Registry<T> &operator=(Registry<T> &other) {
		this->items = other.items;
		return *this;
	}

	void add(T *node) {
		std::unique_lock<std::mutex> lock(mutex);
		if(std::find(items.begin(), items.end(), node) == items.end())
			items.emplace_back(node);
	}
	void remove(T *node) {
		std::unique_lock<std::mutex> lock(mutex);
		if(items.size() > 0)
			items.erase(std::remove(items.begin(), items.end(), node));
	}
	void clear() {
		std::unique_lock<std::mutex> lock(mutex);
		items.clear();
	}
	std::vector<T *> list(bool requireEnabled = false) {
		std::vector<T *> nodes;
		std::unique_lock<std::mutex> lock(mutex);
		for(TypedNodeRef<T> &ref : items) {
			NodeRef nodeRef = ref;
			if(ref && (!requireEnabled || nodeRef.ptr()->getEnabled()))
				nodes.push_back(ref.ptr());
		}
		return nodes;
	}
};

}
