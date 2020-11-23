#include "node.hpp"

namespace StardustXRServer {

void Node::propagate(std::function<void(Node *)> &function) {
	function(this);
	for(const auto &child : children) {
		if(Node *nodeChild = dynamic_cast<Node *>(child.second)) //Check if child node is the StardustXRServer
			nodeChild->propagate(function);
	}
}

} // namespace StardustXRServer
