#include "node.hpp"
#include <vector>

using namespace std;

namespace StardustXRServer {

std::vector<Node *> Node::nodesToDestroy;
std::mutex Node::destroyMutex;

Node::Node(Client *client, bool destroyable) : client(client) {
	this->destroyable = destroyable;
	STARDUSTXR_NODE_METHOD("setEnabled", &Node::setEnabledFlex)
	STARDUSTXR_NODE_METHOD("destroy", &Node::destroyFlex)
}
Node::~Node() {}

void Node::propagate(std::string name, std::function<bool (std::string, Node *)> &function) {
	if(function(name, this)) {
		// Cache names of all children
		for(const auto &child : children) {
			if(child.second)
				child.second->propagate(child.first, function);

			if(children.size() == 0)
				break;
		}
	}
}

std::string Node::hashUUID() {
	std::ostringstream stringStream;
	std::size_t hash = std::hash<std::string>{}(name);
	stringStream << std::uintptr_t((std::uintptr_t) client ^ (std::uintptr_t) hash);
	return stringStream.str();
}

void Node::addChild(std::string name, Node *child) {
	child->parent = this;
	child->name = name;
	children[name] = std::unique_ptr<Node>(child);
}
Node &Node::operator[](const std::string child) {
	return *this->children[child];
}

bool Node::getEnabled() {
	return this->enabled;
}

void Node::setEnabled(bool enabled) {
	this->enabled = enabled;
}

void Node::queueDestroy(bool forceDestroy) {
	if(!forceDestroy && !destroyable)
		return;

	const std::lock_guard<std::mutex> lock(destroyMutex);
	nodesToDestroy.push_back(this);
}

std::vector<uint8_t> Node::setEnabledFlex(flexbuffers::Reference data, bool returnValue) {
	setEnabled(data.AsBool());
	return std::vector<uint8_t>();
}

std::vector<uint8_t> Node::destroyFlex(flexbuffers::Reference data, bool) {
	queueDestroy(false);
	this->enabled = false;
	return std::vector<uint8_t>();
}

void Node::destroyNodes() {
	const std::lock_guard<std::mutex> lock(destroyMutex);
	for(Node *node : nodesToDestroy) {
//        (void) node->parent->children[node->name].release();
		if(node->parent)
			node->parent->children.erase(node->name);
//        delete this;
	}
	nodesToDestroy.clear();
}

} // namespace StardustXRServer
