#pragma once

#include "../nodetypes/core/node.hpp"

namespace StardustXRServer {

class SpatialInterface : public Node {
public:
	SpatialInterface(Client *client);

	std::vector<uint8_t> createSpatial(flexbuffers::Reference data, bool returnValue);
//	std::vector<uint8_t> createZone(flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer
