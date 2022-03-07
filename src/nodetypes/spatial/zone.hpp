#pragma once

#include "spatial.hpp"
#include "../fields/field.hpp"

namespace StardustXRServer {

class Zone : public Spatial {
public:
	Zone(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, Field *field, std::string callbackPath, std::string callbackMethod);
	~Zone();
	Field *field;

	std::vector<uint8_t> isCaptured(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> capture(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> release(Client *callingClient, flexbuffers::Reference data, bool returnValue);

	void releaseSpatial(Spatial *spatial);

	void addSpatial(Spatial *spatial);
	void sendZoneSignals();
private:
	std::vector<Spatial *> oldSpatials;
	std::vector<Spatial *> spatials;

	std::string callbackPath;
	std::string callbackMethod;
};

}
