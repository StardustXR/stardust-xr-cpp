#pragma once

#include <stereokit.h>
#include "../nodetypes/node.hpp"

namespace StardustXRServer {

class RootInterface : public Node {
public:
	RootInterface(Client *client);

	std::vector<uint8_t> disconnect(flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer
