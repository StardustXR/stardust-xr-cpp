#pragma once

#include "noderef.hpp"

#include <mutex>
#include <vector>

namespace StardustXRServer {

template<class T>
class Registry {
	std::mutex mutex;
	std::vector<NodeRef> items;

public:
	void add(T *node) {
		std::unique_lock<std::mutex> lock(mutex);
		items.push_back(NodeRef(node));
	}
	void remove(T *node) {
		std::unique_lock<std::mutex> lock(mutex);
		if(items.size() > 0)
			items.erase(std::remove(items.begin(), items.end(), NodeRef(node)));
	}
	std::vector<T *> list() {
		std::vector<T *> nodes;
		std::unique_lock<std::mutex> lock(mutex);
		for(NodeRef &ref : items) {
			if(ref && ref.ptr<T>())
				nodes.push_back(ref.ptr<T>());
		}
		return nodes;
	}
};

}
