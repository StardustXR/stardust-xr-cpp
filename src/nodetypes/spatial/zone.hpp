#pragma once

#include "spatial.hpp"
#include "../core/callback.hpp"
#include "../fields/field.hpp"

namespace StardustXRServer {

class Zone : public Spatial {
public:
	Zone(Client *client, Spatial *spatialParent, sk::pose_t transform, Field *field, std::string callbackPath, std::string callbackMethod);
	~Zone();
	static Registry<Zone> zones;
	TypedNodeRef<Field> field;

	std::vector<uint8_t> isCaptured(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> captureFlex(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> releaseFlex(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	void capture(std::string uuid);
	void release(std::string uuid);

	void queueSpatial(Spatial *spatial);
	void sendZoneSignals();

	static Registry<Spatial> zoneableSpatials;
	static void updateZones();
private:
	struct CapturedSpatial {
		TypedNodeRef<Spatial> spatial;
		TypedNodeRef<Spatial> originalParent;

		operator bool() {
			return spatial;
		}
	};

	Registry<Spatial> oldInRangeSpatials;
	Registry<Spatial> inRangeSpatials;
	std::map<std::string, TypedNodeRef<Spatial>> inRange;
	std::map<std::string, CapturedSpatial> captured;

	Callback callback;
};

}
