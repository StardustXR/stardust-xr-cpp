#include "node.hpp"
#include <vector>

using namespace std;

namespace StardustXRServer {

void Node::propagate(std::string name, PropagateFunction &function) {
	if(ready && function(name, this)) {
		// Cache names of all children
		vector<string> childNames;
		for(const auto &child : children)
			childNames.push_back(child.first);

		// Delete children using name cache
		for(const auto &childName : childNames) {
			Node *child = children.at(childName);
			if(child)
				child->propagate(childName, function);

			if(children.size() == 0)
				break;
		}
	}
}

} // namespace StardustXRServer
