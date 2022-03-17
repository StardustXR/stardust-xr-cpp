#pragma once

#include "../nodetypes/spatial/spatial.hpp"
#include "../nodetypes/spatial/zone.hpp"
#include "../nodetypes/core/registry.hpp"

namespace StardustXRServer {

class SpatialInterface : public Node {
public:
	SpatialInterface(Client *client);

	std::vector<uint8_t> createSpatial(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> createZone(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	static void updateZones();
};

} // namespace StardustXRServer
