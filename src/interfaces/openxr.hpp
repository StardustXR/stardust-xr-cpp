#pragma once

#include "../nodetypes/core/node.hpp"
#include <memory>
#include <map>
#include <openxr/openxr.h>

namespace StardustXRServer {

class OpenXRInstance;
class OpenXRSystem;

class OpenXRInterface : public Node {
public:
	OpenXRInterface(Client *client);

	std::vector<uint8_t> createInstance(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	const OpenXRSystem *system;
protected:
	OpenXRInstance *instance = nullptr;
};

} // namespace StardustXRServer
