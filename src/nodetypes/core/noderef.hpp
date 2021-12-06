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
	NodeRef(NodeRef &);
	NodeRef(uint32_t id);
	~NodeRef();

	operator bool() const;
	Node *operator ->() const;
	operator Node*() const;

	Node *ptr() const;
	static uint32_t registerNode(Node *node);

protected:
	const uint32_t id;

	static uint32_t generateUUID();
	static std::mutex registryMutex;
	static std::unordered_map<uint32_t, Node *> registry;
};

}
