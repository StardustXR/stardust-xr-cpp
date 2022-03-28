#pragma once

#include "../core/noderef.hpp"
#include "../spatial/spatial.hpp"

namespace StardustXRServer {

class PulseSender : public Spatial {
public:
	PulseSender(Client *client, Spatial *spatialParent, sk::pose_t pose);
	static Registry<PulseSender> senders;
	~PulseSender();

	std::vector<uint8_t> getReceivers(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> sendData(Client *callingClient, flexbuffers::Reference data, bool returnValue);
};

}
