#pragma once

#include <functional>
#include <stardustxr/server/node.hpp>
#include <stardustxr/util.hpp>

using namespace StardustXR;
namespace StardustXRServer {

class Node;
class Client;

class Node : public ServerNode {
public:
	Node(Client *client);
	virtual ~Node() {}

	virtual void update() {}
	virtual void handleMessengerDeletion(uint sessionID) {}
	void propagate(std::string name, std::function<bool (std::string, Node *)> &function);

	bool ready = true;
	uint sessionID = 0;
	Client *client;
	Node *parent = nullptr;
	std::map<std::string, Node *> children;
};

} //namespace StardustXRServer