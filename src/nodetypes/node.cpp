#include "node.hpp"
#include <vector>

using namespace std;

namespace StardustXRServer {

void Node::propagate(std::string name, PropagateFunction &function) {
	if(ready && function(name, this)) {
		// Cache names of all children
		vector<string> childNames;
		for(const auto &child : children) {
			child.second->propagate(child.first, function);

			if(children.size() == 0)
				break;
		}
	}
}

} // namespace StardustXRServer
