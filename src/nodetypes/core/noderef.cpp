#include "noderef.hpp"
#include "node.hpp"
#include "../../util/random.hpp"

namespace StardustXRServer {
std::mutex NodeRef::registryMutex;
std::unordered_map<uint32_t, Node *> NodeRef::registry;

NodeRef::NodeRef() :
	id(0) {}
NodeRef::NodeRef(const NodeRef &original):
	id(original.id) {}
NodeRef::NodeRef(uint32_t id) :
	id(id) {}
NodeRef::NodeRef(Node *node) :
	id(node == nullptr ? 0 : node->id) {}

NodeRef &NodeRef::operator=(NodeRef other) {
	this->id = other.id;
	return *this;
}

NodeRef::operator bool() const {
	std::lock_guard<std::mutex> lock(registryMutex);
	return registry.find(id) != registry.end();
}

Node *NodeRef::operator ->() const {
	return ptr();
}
NodeRef::operator Node *() const {
	return ptr();
}
Node *NodeRef::ptr() const {
	std::lock_guard<std::mutex> lock(registryMutex);
	auto registryEntry = registry.find(id);
	return registryEntry == registry.end() ? nullptr : registryEntry->second;
}

uint32_t NodeRef::registerNode(Node *node) {
	uint32_t uuid = generateUUID();
	std::lock_guard<std::mutex> lock(registryMutex);
	registry[uuid] = node;
	return uuid;
}

void NodeRef::deregisterNode(Node *node) {
	std::lock_guard<std::mutex> lock(registryMutex);
	registry.erase(registry.find(node->id));
}

uint32_t NodeRef::generateUUID() {
	std::lock_guard<std::mutex> lock(registryMutex);
	uint32_t id = 0;
	while(id == 0 || registry.find(id) != registry.end()) {
		id = Random::generate<uint32_t>();
	}
	return id;
}

}
