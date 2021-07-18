#include "node.hpp"
#include <vector>

using namespace std;

namespace StardustXRServer {

Node::Node(Client *client) : client(client) {}
Node::~Node() {}

void Node::propagate(std::string name, std::function<bool (std::string, Node *)> &function) {
	if(ready && function(name, this)) {
		// Cache names of all children
		for(const auto &child : children) {
			if(child.second)
				child.second->propagate(child.first, function);

			if(children.size() == 0)
				break;
		}
	}
}

} // namespace StardustXRServer
