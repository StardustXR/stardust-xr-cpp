#include "alias.hpp"

namespace StardustXRServer {

Alias::Alias(Client *client, Node *original, std::vector<std::string> methods) : Node(client, false), original(original) {
	original->aliases.push_back(this);

	addMethods(methods);
}

void Alias::addMethods(std::vector<std::string> methods) {
	for(std::string &method : methods) {
		if(original->methods.find(method) != original->methods.end())
			this->methods[method] = original->methods[method];
	}
}

} // namespace StardustXRServer
