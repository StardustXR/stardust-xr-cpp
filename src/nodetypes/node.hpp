#pragma once

#include <functional>
#include <stardustxr/node.hpp>

using namespace StardustXR;
namespace StardustXRServer {

class Node;
class Client;

class Node : public StardustXR::Node {
public:
	Node(Client *client);
	virtual ~Node();

	virtual void update() {}
	virtual void handleClientDisconnect(Client *client) {}
	void propagate(std::string name, std::function<bool (std::string, Node *)> &function);

	bool ready = true;
	Client *client;
	Node *parent = nullptr;
	std::map<std::string, Node *> children;
};

} //namespace StardustXRServer