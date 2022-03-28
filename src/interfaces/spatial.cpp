#include "spatial.hpp"
#include "../core/client.hpp"
#include "../nodetypes/spatial/spatial.hpp"
#include "../nodetypes/spatial/zone.hpp"
#include "../util/flex.hpp"

#include <list>
#include <limits>

using namespace sk;
using namespace std;

namespace StardustXRServer {

SpatialInterface::SpatialInterface(Client *client) : Node(client, false) {
	STARDUSTXR_NODE_METHOD("createSpatial", &SpatialInterface::createSpatial)
	STARDUSTXR_NODE_METHOD("createZone", &SpatialInterface::createZone)
	addChild("spatial", new Node(client));
	addChild("zone", new Node(client));
}

std::vector<uint8_t> SpatialInterface::createSpatial(Client *callingClient, flexbuffers::Reference data, bool) {
	flexbuffers::Vector flexVec = data.AsVector();
	string name                 = flexVec[0].AsString().str();
	Spatial *spatialParent      = this->client->scenegraph.findNode<Spatial>(flexVec[1].AsString().str());
	matrix transform            = FlexToSKTransform(flexVec[2].AsTypedVector(), flexVec[3].AsTypedVector(), flexVec[4].AsTypedVector());
	bool translatable           = flexVec[5].AsBool();
	bool rotatable              = flexVec[6].AsBool();
	bool scalable               = flexVec[7].AsBool();
	bool zoneable               = flexVec[8].AsBool();

	Spatial *spatial = new Spatial(client, spatialParent, transform, translatable, rotatable, scalable, zoneable);
	children["spatial"]->addChild(name, spatial);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> SpatialInterface::createZone(Client *callingClient, flexbuffers::Reference data, bool) {
	flexbuffers::Vector flexVec = data.AsVector();
	string name                 = flexVec[0].AsString().str();
	Field *field                = callingClient->scenegraph.findNode<Field>(flexVec[1].AsString().str());
	Spatial *spatialParent      = callingClient->scenegraph.findNode<Spatial>(flexVec[2].AsString().str());
	pose_t transform            = FlexToSKPose(flexVec[3].AsTypedVector(), flexVec[4].AsTypedVector());
	std::string callbackPath    = flexVec[5].AsString().str();
	std::string callbackMethod  = flexVec[6].AsString().str();

	Zone *zone = new Zone(client, spatialParent, transform, field, callbackPath, callbackMethod);
	children["zone"]->addChild(name, zone);

	return std::vector<uint8_t>();
}

void SpatialInterface::updateZones() {
	for(Spatial *spatial : Spatial::spatials.list()) {
		if(spatial->zoneable && spatial->getEnabled()) {
			float spatialDistance = spatial->zone ? spatial->zone->field->distance(spatial, vec3_zero) : 1.0f;
			for(Zone *zone : Zone::zones.list()) {
				if(zone->field == nullptr || zone == spatial || spatial == zone->field)
					continue;
				float distance = zone->field->distance(spatial, vec3_zero);
				if(distance < 0 && distance <= spatialDistance) {
					zone->addSpatial(spatial);
				}
			}
		}
	}
	for(Zone *zone : Zone::zones.list()) {
		zone->sendZoneSignals();
	}
}

} // namespace StardustXRServer
