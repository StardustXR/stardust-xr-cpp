#pragma once

#include "../core/noderef.hpp"
#include "../spatial/spatial.hpp"

namespace StardustXRServer {

class NonSpatialSender : public Spatial {
public:
	NonSpatialSender(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation);
	~NonSpatialSender();

	static std::mutex sendersMutex;
	static std::vector<NonSpatialSender *> senders;

	std::vector<uint8_t> getReceivers(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> sendData(Client *callingClient, flexbuffers::Reference data, bool returnValue);
};

}
