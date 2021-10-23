#include "zone.hpp"

#include <stardustxr/common/flex.hpp>

#include "../../core/client.hpp"
#include "../../interfaces/spatial.hpp"
#include "../../nodetypes/core/alias.hpp"

namespace StardustXRServer {

Zone::Zone(Client *client, Spatial *spatialParent, vec3 position, quat rotation, Field *field, std::string callbackPath, std::string callbackMethod) :
	Spatial(client, spatialParent, position, rotation, vec3_one, true, true, false, false) {
	this->field = field;
	this->callbackPath = callbackPath;
	this->callbackMethod = callbackMethod;

	STARDUSTXR_NODE_METHOD("isCaptured", &Zone::isCaptured)
	STARDUSTXR_NODE_METHOD("capture", &Zone::capture)
	STARDUSTXR_NODE_METHOD("release", &Zone::release)

	std::lock_guard<std::mutex> lock(SpatialInterface::spatialMutex);
	SpatialInterface::zones.push_back(this);
}
Zone::~Zone() {
	for(auto &childSet : children) {
		Alias *child = dynamic_cast<Alias *>(childSet.second.get());
		if(child != nullptr) {
			Spatial *childSpatial = dynamic_cast<Spatial *>(child->getOriginal());
			releaseSpatial(childSpatial);
		}
	}
	std::lock_guard<std::mutex> lock(SpatialInterface::spatialMutex);
	SpatialInterface::zones.erase(std::remove(SpatialInterface::zones.begin(), SpatialInterface::zones.end(), this));
}

std::vector<uint8_t> Zone::isCaptured(flexbuffers::Reference data, bool returnValue) {
	Alias *spatialAlias = dynamic_cast<Alias *>(this->children[data.AsString().str()].get());
	if(spatialAlias == nullptr)
		return StardustXR::FlexbufferFromArguments(FLEX_ARG(FLEX_NULL));
	Spatial *spatialOriginal = dynamic_cast<Spatial *>(spatialAlias->getOriginal());
	return StardustXR::FlexbufferFromArguments(FLEX_ARG(FLEX_BOOL(spatialOriginal->zone != nullptr)));
}

std::vector<uint8_t> Zone::capture(flexbuffers::Reference data, bool returnValue) {
	std::string uuid = data.AsString().str();
	Alias *spatialAlias = dynamic_cast<Alias *>(this->children[uuid].get());
	if(spatialAlias == nullptr)
		return std::vector<uint8_t>();

	Spatial *spatialOriginal = dynamic_cast<Spatial *>(spatialAlias->getOriginal());
	spatialOriginal->zone = this;
	spatialOriginal->originalSpatialParent = spatialOriginal->spatialParent;
	spatialOriginal->setSpatialParentInPlace(this);

	return std::vector<uint8_t>();
}
std::vector<uint8_t> Zone::release(flexbuffers::Reference data, bool returnValue) {
	Alias *spatialAlias = dynamic_cast<Alias *>(this->children[data.AsString().str()].get());
	if(spatialAlias == nullptr)
		return std::vector<uint8_t>();

	Spatial *spatialOriginal = dynamic_cast<Spatial *>(spatialAlias->getOriginal());
	releaseSpatial(spatialOriginal);
	return std::vector<uint8_t>();
}

void Zone::releaseSpatial(Spatial *spatial) {
	if(spatial->zone == this) {
		spatial->zone = nullptr;
		spatial->setSpatialParentInPlace(spatial->originalSpatialParent);
	}
}

void Zone::addSpatial(Spatial *spatial) {
	spatials.push_back(spatial);
}

void Zone::sendZoneSignals() {
	std::vector<Spatial *> enter;
	std::vector<Spatial *> leave;
	std::sort(spatials.begin(), spatials.end(), [](Spatial* a, Spatial* b) {
		return ((std::uintptr_t) a) < ((std::uintptr_t) b);
	});
	std::set_difference(spatials.begin(), spatials.end(),
						oldSpatials.begin(), oldSpatials.end(),
						std::inserter(enter, enter.begin()));
	std::set_difference(oldSpatials.begin(), oldSpatials.end(),
						spatials.begin(), spatials.end(),
						std::inserter(leave, leave.begin()));

	client->messenger.sendSignal(
		callbackPath.c_str(),
		callbackMethod.c_str(),
		[&](flexbuffers::Builder &fbb) {
			fbb.Vector([&] {
				fbb.TypedVector([&] {
					for(Spatial *enterNode : enter) {
						std::string uuid = enterNode->hashUUID();
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
						std::string uuid = leaveNode->hashUUID();
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
