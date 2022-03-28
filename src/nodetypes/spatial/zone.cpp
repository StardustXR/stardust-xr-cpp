#include "zone.hpp"

#include <stardustxr/common/flex.hpp>

#include "../../core/client.hpp"
#include "../../interfaces/spatial.hpp"
#include "../../nodetypes/core/alias.hpp"

using namespace sk;

namespace StardustXRServer {

Registry<Zone> Zone::zones;

Zone::Zone(Client *client, Spatial *spatialParent, pose_t transform, Field *field, std::string callbackPath, std::string callbackMethod) :
Spatial(client, spatialParent, transform, true, true, false, false),
callback({client, callbackPath, callbackMethod}) {
	this->field = field;

	STARDUSTXR_NODE_METHOD("isCaptured", &Zone::isCaptured)
	STARDUSTXR_NODE_METHOD("capture", &Zone::capture)
	STARDUSTXR_NODE_METHOD("release", &Zone::release)

	zones.add(this);
}
Zone::~Zone() {
	for(auto &childSet : children) {
		Alias *child = dynamic_cast<Alias *>(childSet.second.get());
		if(child != nullptr) {
			Spatial *childSpatial = child->original.ptr<Spatial>();
			releaseSpatial(childSpatial);
		}
	}
	zones.remove(this);
}

std::vector<uint8_t> Zone::isCaptured(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	Alias *spatialAlias = dynamic_cast<Alias *>(this->children[data.AsString().str()].get());
	if(spatialAlias == nullptr)
		return StardustXR::FlexbufferFromArguments(FLEX_ARG(FLEX_NULL));
	Spatial *spatialOriginal = spatialAlias->original.ptr<Spatial>();
	return StardustXR::FlexbufferFromArguments(FLEX_ARG(FLEX_BOOL(spatialOriginal->zone != nullptr)));
}

std::vector<uint8_t> Zone::capture(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	std::string uuid = data.AsString().str();
	Alias *spatialAlias = dynamic_cast<Alias *>(this->children[uuid].get());
	if(spatialAlias == nullptr)
		return std::vector<uint8_t>();

	Spatial *spatialOriginal = spatialAlias->original.ptr<Spatial>();
	spatialOriginal->zone = this;
	spatialOriginal->originalSpatialParent = spatialOriginal->spatialParent;
	spatialOriginal->setSpatialParentInPlace(this);

	return std::vector<uint8_t>();
}
std::vector<uint8_t> Zone::release(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	Alias *spatialAlias = dynamic_cast<Alias *>(this->children[data.AsString().str()].get());
	if(spatialAlias == nullptr)
		return std::vector<uint8_t>();

	Spatial *spatialOriginal = spatialAlias->original.ptr<Spatial>();
	releaseSpatial(spatialOriginal);
	return std::vector<uint8_t>();
}

void Zone::releaseSpatial(Spatial *spatial) {
	if(spatial && spatial->zone == this) {
		spatial->zone = nullptr;
		spatial->setSpatialParentInPlace(spatial->originalSpatialParent);
	}
}

void Zone::addSpatial(Spatial *spatial) {
	spatials.add(spatial);
}

void Zone::sendZoneSignals() {
	std::vector<Spatial *> enter;
	std::vector<Spatial *> leave;
	std::vector<Spatial *> spatials = this->spatials.list();
	std::vector<Spatial *> oldSpatials = this->oldSpatials.list();
//	std::sort(spatials.begin(), spatials.end(), [](Spatial* a, Spatial* b) {
//		return ((std::uintptr_t) a) < ((std::uintptr_t) b);
//	});

	std::set_difference(spatials.begin(), spatials.end(),
						oldSpatials.begin(), oldSpatials.end(),
						std::inserter(enter, enter.begin()));
	std::set_difference(oldSpatials.begin(), oldSpatials.end(),
						spatials.begin(), spatials.end(),
						std::inserter(leave, leave.begin()));

	callback.signal(
		[&](flexbuffers::Builder &fbb) {
			fbb.Vector([&] {
				fbb.TypedVector([&] {
					for(Spatial *enterNode : enter) {
						std::string uuid = std::to_string(enterNode->id);
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
						fbb.String(uuid);
					}
				});
				fbb.TypedVector([&] {
					for(Spatial *leaveNode : leave) {
						releaseSpatial(leaveNode);
						std::string uuid = std::to_string(leaveNode->id);
						if(children[uuid] != nullptr)
							children.erase(uuid);
						fbb.String(uuid);
					}
				});
			});
		}
	);

	oldSpatials = spatials;
	spatials.clear();
}

}
