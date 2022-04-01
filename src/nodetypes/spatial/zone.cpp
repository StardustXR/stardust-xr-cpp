#include "zone.hpp"

#include <stardustxr/common/flex.hpp>

#include "../../core/client.hpp"
#include "../../interfaces/spatial.hpp"
#include "../../nodetypes/core/alias.hpp"

using namespace sk;

namespace StardustXRServer {

Registry<Zone> Zone::zones;
Registry<Spatial> Zone::zoneableSpatials;

Zone::Zone(Client *client, Spatial *spatialParent, pose_t transform, Field *field, std::string callbackPath, std::string callbackMethod) :
Spatial(client, spatialParent, transform, true, true, false, false),
callback({client, callbackPath, callbackMethod}) {
	this->field = field;

	STARDUSTXR_NODE_METHOD("isCaptured", &Zone::isCaptured)
	STARDUSTXR_NODE_METHOD("capture", &Zone::captureFlex)
	STARDUSTXR_NODE_METHOD("release", &Zone::releaseFlex)

	zones.add(this);
}
Zone::~Zone() {
	for(auto capturedSpatial : captured) {
		release(capturedSpatial.first);
	}
	zones.remove(this);
}

std::vector<uint8_t> Zone::isCaptured(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	std::string uuid = data.AsString().str();
	return FLEX_SINGLE(FLEX_BOOL(captured[uuid]));
}

std::vector<uint8_t> Zone::captureFlex(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	std::string uuid = data.AsString().str();
	capture(uuid);

	return std::vector<uint8_t>();
}
std::vector<uint8_t> Zone::releaseFlex(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	std::string uuid = data.AsString().str();
	release(uuid);

	return std::vector<uint8_t>();
}

void Zone::capture(std::string uuid) {
	if(this->inRange.count(uuid) == 0 && this->captured.count(uuid) == 0)
		return;
	TypedNodeRef<Spatial> spatial = inRange[uuid];
	if(spatial) {
		this->captured[uuid] = {
			spatial,
			spatial->getSpatialParent()
		};
		spatial->zone = this;
		spatial->setSpatialParentInPlace(this);
	}
}
void Zone::release(std::string uuid) {
	if(this->captured.count(uuid) == 0)
		return;
	CapturedSpatial captured = this->captured[uuid];
	if(captured) {
		Spatial *newParent = captured.originalParent ? captured.originalParent.ptr() : &captured.spatial->client->scenegraph.root;
		captured.spatial->zone = nullptr;
		captured.spatial->setSpatialParentInPlace(newParent);
	}
	this->captured.erase(uuid);
}

void Zone::queueSpatial(Spatial *spatial) {
	inRangeSpatials.add(spatial);
}

void Zone::updateZones() {
	// Loop through all spatials in the zoneable registry that are enabled
	for(Spatial *spatial : Zone::zoneableSpatials.list(true)) {
		// Get the distance between the spatial and its zone if it has one, otherwise return 1 ensuring any zone will be closer
		float oldZoneDistance = spatial->zone ? spatial->zone->field->distance(spatial, vec3_zero) : 1.0f;
		// Loop through all zones in the zone registry that are enabled
		for(Zone *zone : Zone::zones.list(true)) {
			// Try the next zone if the zone doesn't have a field, or the zone tries to include itself/its field
			if(!zone->field || zone == spatial || spatial == zone->field)
				continue;
			// Get the distance between the spatial's origin and this zone
			float distance = zone->field->distance(spatial, vec3_zero);
			// Only queue up spatials that are inside the zone or closer to this zone than their currently captured zone
			if(distance < 0 && distance <= oldZoneDistance)
				zone->queueSpatial(spatial);
		}
	}
	for(Zone *zone : Zone::zones.list()) {
		zone->sendZoneSignals();
	}
}

void Zone::sendZoneSignals() {
	std::vector<Spatial *> enter;
	std::vector<Spatial *> leave;
	std::vector<Spatial *> spatials = this->inRangeSpatials.list();
	std::vector<Spatial *> oldSpatials = this->oldInRangeSpatials.list();

	std::set_difference(spatials.begin(), spatials.end(),
						oldSpatials.begin(), oldSpatials.end(),
						std::inserter(enter, enter.begin()));
	std::set_difference(oldSpatials.begin(), oldSpatials.end(),
						spatials.begin(), spatials.end(),
						std::inserter(leave, leave.begin()));

	callback.signal(
		FLEX_ARGS(
			FLEX_TYPED_VEC(
				for(Spatial *enterNode : enter) {
					std::string uuid = std::to_string(enterNode->id);
					inRange[uuid] = enterNode;
					addChild(uuid, new Alias(client, enterNode, {
						"move",
						"rotate",
						"scale",
						"setOrigin",
						"setOrientation",
						"setScale",
						"setPose",
						"setTransform"
					}));
					FLEX_STRING(uuid);
				}
			)
			FLEX_TYPED_VEC(
				for(Spatial *leaveNode : leave) {
					std::string uuid = std::to_string(leaveNode->id);
					release(uuid);
					inRange.erase(uuid);
					if(children[uuid] != nullptr)
						children.erase(uuid);
					FLEX_STRING(uuid);
				}
			)
		)
	);

	oldInRangeSpatials = inRangeSpatials;
	inRangeSpatials.clear();
}

}
