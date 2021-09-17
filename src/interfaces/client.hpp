#pragma once

#include <stereokit.h>
#include "../nodetypes/node.hpp"

namespace StardustXRServer {

class ClientInterface : public Node {
public:
	ClientInterface(Client *client);
	virtual ~ClientInterface() {}

	std::vector<uint8_t> disconnect(flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer
