#pragma once

#include "../../nodetypes/core/node.hpp"

#include <openxr/openxr.h>
#include <string>
#include <stdint.h>
#include <vector>

namespace StardustXRServer {

class OpenXRSystem : public Node {
public:
	OpenXRSystem(Client *client, XrFormFactor formFactor);

	const XrFormFactor formFactor;
	std::vector<uint8_t> getViewConfigurations(Client *callingClient, flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer
