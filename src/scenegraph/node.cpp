#include "node.hpp"

namespace StardustXRServer {

void Node::propagateUpdate() {
	for(const auto child : children) {
		if(Node *nodeChild = dynamic_cast<Node *>(child.second)) { //Check if child node is the StardustXRServer node type
			nodeChild->update();
			nodeChild->propagateUpdate();
		}
	}
}

} // namespace StardustXRServer
