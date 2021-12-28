#pragma once

#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <flatbuffers/flexbuffers.h>

namespace StardustXRServer {

#define STARDUSTXR_NODE_METHOD(method_name, method_ref) this->methods[method_name] = std::bind(method_ref, this, std::placeholders::_1, std::placeholders::_2);
typedef std::function<std::vector<uint8_t>(flexbuffers::Reference, bool)> NodeMethod;

class Node;
class Alias;
class Client;

class Node {
public:
	Node(Client *client, bool destroyable = true);
	virtual ~Node();

	virtual void update() {}
	virtual void debug() {}
	void propagate(std::string name, std::function<bool (std::string, Node *)> &function);
	
	Client *client;
	Node *parent = nullptr;
	std::string name = "";
	const uint32_t id;

	void addChild(std::string name, Node *child);
	Node &operator[](const std::string child);
	bool getEnabled();
	void setEnabled(bool enabled);
    void queueDestroy(bool forceDestroy);
    static void destroyNodes();

	std::vector<uint8_t> setEnabledFlex(flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> destroyFlex(flexbuffers::Reference data, bool returnValue);

	std::map<std::string, NodeMethod> methods;
	std::map<std::string, std::unique_ptr<Node>> children;
	std::vector<Alias *> aliases;
protected:
	std::atomic_bool enabled = {true};

    static std::vector<Node *> nodesToDestroy;
    static std::mutex destroyMutex;
	bool destroyable = true;
	bool destroyQueued = false;
};

} //namespace StardustXRServer
