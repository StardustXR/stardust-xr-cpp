#include "node.hpp"

namespace StardustXRServer {

void Node::propagate(std::string name, PropagateFunction &function) {
	if(function(name, this)) {
		for(const auto &child : children) {
			child.second->propagate(child.first, function);

			if(children.size() == 0)
				break;
		}
	}
}

} // namespace StardustXRServer