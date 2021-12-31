#pragma once

#include "node.hpp"
#include <vector>

namespace StardustXRServer {

class Alias : public Node {
public:
	Alias(Client *client, Node *original, std::vector<std::string> methods);
	~Alias();

	void addMethods(std::vector<std::string> methods);

	Node *const original;
};

} // namespace StardustXRServer
