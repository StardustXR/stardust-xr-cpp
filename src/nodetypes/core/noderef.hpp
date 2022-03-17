#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>
#include <stdint.h>

namespace StardustXRServer {

class Node;

class NodeRef {
public:
	NodeRef();
	NodeRef(const NodeRef &);
	NodeRef(uint32_t id);
	NodeRef(Node *node);

	NodeRef &operator=(NodeRef other);
	operator bool() const;
	Node *operator ->() const;
	operator Node*() const;
	bool operator==(const Node *other) const;
	bool operator!=(const Node *other) const;
	bool operator==(const NodeRef &other) const;
	bool operator!=(const NodeRef &other) const;

	Node *ptr() const;
	template<class T>
	T *ptr() const {
		Node *node = ptr();
		if(node)
			return dynamic_cast<T *>(node);
		else
			return nullptr;
	}

	static uint32_t registerNode(Node *node);
	static void deregisterNode(Node *node);

protected:
	uint32_t id;

	static uint32_t generateUUID();
	static std::mutex registryMutex;
	static std::unordered_map<uint32_t, Node *> registry;
};

}
