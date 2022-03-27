#pragma once

#include "../core/noderef.hpp"
#include "../spatial/spatial.hpp"

namespace StardustXRServer {

class NonSpatialSender : public Spatial {
public:
	NonSpatialSender(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation);
	static Registry<NonSpatialSender> senders;
	~NonSpatialSender();

	std::vector<uint8_t> getReceivers(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> sendData(Client *callingClient, flexbuffers::Reference data, bool returnValue);
};

}