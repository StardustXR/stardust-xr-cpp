#ifndef STARDUSTXR_NODE_HPP
#define STARDUSTXR_NODE_HPP

#include <functional>
#include <stardustxr/server/messengermanager.hpp>
#include <stardustxr/server/node.hpp>
#include <stardustxr/util.hpp>

using namespace StardustXR;
namespace StardustXRServer {

class Node;
class Scenegraph;

typedef std::function<bool (std::string, Node *)> PropagateFunction;

class Node : public ServerNode {
public:
	Node() {}
	virtual ~Node() {}

	virtual void update() {}
	virtual void handleMessengerDeletion(uint sessionID) {}
	void propagate(std::string name, PropagateFunction &function);

	uint sessionID = 0;
	Node *parent = nullptr;
	std::map<std::string, Node *> children;
};

} //namespace StardustXRServer

#endif //STARDUSTXR_NODE_HPP
