#pragma once

#include "../nodetypes/core/node.hpp"
#include <memory>

namespace StardustXRServer {

class OpenXRInstance;

class OpenXRInterface : public Node {
public:
	OpenXRInterface(Client *client);

	std::vector<uint8_t> createInstance(Client *callingClient, flexbuffers::Reference data, bool returnValue);

protected:
	OpenXRInstance *instance = nullptr;
};

} // namespace StardustXRServer
