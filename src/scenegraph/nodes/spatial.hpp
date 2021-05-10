#pragma once

#include "../../nodetypes/node.hpp"

namespace StardustXRServer {

class SpatialFactory : public Node {
public:
	SpatialFactory();
	virtual ~SpatialFactory() {}

	std::vector<uint8_t> create(uint sessionID, flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer
