#pragma once

#include "../nodetypes/spatial/spatial.hpp"
#include "../nodetypes/spatial/zone.hpp"

namespace StardustXRServer {

class SpatialInterface : public Node {
public:
	SpatialInterface(Client *client);

	std::vector<uint8_t> createSpatial(flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> createZone(flexbuffers::Reference data, bool returnValue);

	static void updateZones();

	static std::mutex spatialMutex;
	static std::vector<Spatial *> spatials;
	static std::vector<Zone *> zones;
};

} // namespace StardustXRServer
