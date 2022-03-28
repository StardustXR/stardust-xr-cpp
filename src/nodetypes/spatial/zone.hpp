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
	Field *field;

	std::vector<uint8_t> isCaptured(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> capture(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> release(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	void releaseSpatial(Spatial *spatial);

	void addSpatial(Spatial *spatial);
	void sendZoneSignals();
private:
	Registry<Spatial> oldSpatials;
	Registry<Spatial> spatials;

	Callback callback;
};

}
