#include "spatial.hpp"
#include "../core/client.hpp"
#include "../nodetypes/spatial/spatial.hpp"
#include "../nodetypes/spatial/zone.hpp"

#include <list>
#include <limits>

using namespace sk;
using namespace std;

namespace StardustXRServer {

std::mutex SpatialInterface::spatialMutex;
std::vector<Spatial *> SpatialInterface::spatials;
std::vector<Zone *> SpatialInterface::zones;

SpatialInterface::SpatialInterface(Client *client) : Node(client, false) {
	STARDUSTXR_NODE_METHOD("createSpatial", &SpatialInterface::createSpatial)
	STARDUSTXR_NODE_METHOD("createZone", &SpatialInterface::createZone)
	addChild("spatial", new Node(client));
	addChild("zone", new Node(client));
}

std::vector<uint8_t> SpatialInterface::createSpatial(flexbuffers::Reference data, bool) {
	flexbuffers::Vector vector            = data.AsVector();
	string name                           = vector[0].AsString().str();
	Spatial *spatialParent                = this->client->scenegraph.findNode<Spatial>(vector[1].AsString().str());
	flexbuffers::TypedVector flexPosition = vector[2].AsTypedVector();
	flexbuffers::TypedVector flexRotation = vector[3].AsTypedVector();
	flexbuffers::TypedVector flexScale    = vector[4].AsTypedVector();
	bool translatable                     = vector[5].AsBool();
	bool rotatable                        = vector[6].AsBool();
	bool scalable                         = vector[7].AsBool();
	bool zoneable                         = vector[8].AsBool();

	vec3 position = {
		flexPosition[0].AsFloat(),
		flexPosition[1].AsFloat(),
		flexPosition[2].AsFloat()
	};
	quat rotation = {
		flexRotation[0].AsFloat(),
		flexRotation[1].AsFloat(),
		flexRotation[2].AsFloat(),
		flexRotation[3].AsFloat()
	};
	vec3 scale = {
		flexScale[0].AsFloat(),
		flexScale[1].AsFloat(),
		flexScale[2].AsFloat()
	};

	Spatial *spatial = new Spatial(client, spatialParent, position, rotation, scale, translatable, rotatable, scalable, zoneable);
	children["spatial"]->addChild(name, spatial);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> SpatialInterface::createZone(flexbuffers::Reference data, bool) {
	flexbuffers::Vector vector            = data.AsVector();
	string name                           = vector[0].AsString().str();
	Field *field                          = this->client->scenegraph.findNode<Field>(vector[1].AsString().str());
	Spatial *spatialParent                = this->client->scenegraph.findNode<Spatial>(vector[2].AsString().str());
	flexbuffers::TypedVector flexPosition = vector[3].AsTypedVector();
	flexbuffers::TypedVector flexRotation = vector[4].AsTypedVector();
	std::string callbackPath              = vector[5].AsString().str();
	std::string callbackMethod            = vector[6].AsString().str();

	vec3 position = {
		flexPosition[0].AsFloat(),
		flexPosition[1].AsFloat(),
		flexPosition[2].AsFloat()
	};
	quat rotation = {
		flexRotation[0].AsFloat(),
		flexRotation[1].AsFloat(),
		flexRotation[2].AsFloat(),
		flexRotation[3].AsFloat()
	};

	Zone *zone = new Zone(client, spatialParent, position, rotation, field, callbackPath, callbackMethod);
	children["zone"]->addChild(name, zone);

	return std::vector<uint8_t>();
}

void SpatialInterface::updateZones() {
	const std::lock_guard<std::mutex> lock(spatialMutex);

	for(Spatial *spatial : spatials) {
		if(spatial->zoneable) {
			float spatialDistance = spatial->zone ? spatial->zone->field->distance(spatial, vec3_zero) : 1.0f;
			for(Zone *zone : zones) {
				if(zone->field == nullptr || zone == spatial || spatial == zone->field)
					continue;
				float distance = zone->field->distance(spatial, vec3_zero);
				if(distance < 0 && distance <= spatialDistance) {
					zone->addSpatial(spatial);
				}
			}
		}
	}
	for(Zone *zone : zones) {
		zone->sendZoneSignals();
	}
}

} // namespace StardustXRServer
