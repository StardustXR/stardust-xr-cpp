#pragma once

#include "../nodetypes/core/node.hpp"

namespace StardustXRServer {

class SpatialFactory : public Node {
public:
	SpatialFactory(Client *client);
	virtual ~SpatialFactory() {}

	std::vector<uint8_t> create(flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer
