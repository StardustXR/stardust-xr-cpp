#include "node.hpp"

namespace StardustXRServer {

void Node::propagate(std::string name, PropagateFunction &function) {
	if(ready && function(name, this)) {
		for(const auto &child : children) {
			if(child.second)
				child.second->propagate(child.first, function);

			if(children.size() == 0)
				break;
		}
	}
}

} // namespace StardustXRServer
