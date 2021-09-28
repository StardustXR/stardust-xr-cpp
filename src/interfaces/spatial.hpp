#pragma once

#include "../nodetypes/core/node.hpp"

namespace StardustXRServer {

class SpatialFactory : public Node {
public:
	SpatialFactory(Client *client);
	virtual ~SpatialFactory() {}

	std::vector<uint8_t> createSpatial(flexbuffers::Reference data, bool returnValue);
//	std::vector<uint8_t> createZone(flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer
