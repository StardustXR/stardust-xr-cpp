#pragma once;

#include "../nodetypes/core/node.hpp"

namespace StardustXRServer {

class DataInterface : public Node {
public:
	DataInterface(Client *client);

	std::vector<uint8_t> createNonSpatialSender(flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> createNonSpatialReceiver(flexbuffers::Reference data, bool returnValue);
};

}
