#pragma once;

#include "../nodetypes/core/node.hpp"

namespace StardustXRServer {

class DataInterface : public Node {
public:
	DataInterface(Client *client);

	std::vector<uint8_t> createPulseSender(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> createPulseReceiver(Client *callingClient, flexbuffers::Reference data, bool returnValue);
};

}
