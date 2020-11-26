#ifndef STARDUSTXR_NODE_HPP
#define STARDUSTXR_NODE_HPP

#include <functional>
#include <stardustxr/server/node.hpp>
#include <stardustxr/util.hpp>

using namespace StardustXR;
namespace StardustXRServer {

class Node : public ServerNode {
public:
	Node() {}
	virtual ~Node() {}

	virtual void update() {}
	void propagate(std::function<void(Node *)> &function);

	int sessionID = 0;
	std::map<std::string, Node *> children;
};

} //namespace StardustXRServer

#endif //STARDUSTXR_NODE_HPP
