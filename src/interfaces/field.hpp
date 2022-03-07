#pragma once

#include "../nodetypes/core/node.hpp"

namespace StardustXRServer {

class FieldInterface : public Node {
public:
	FieldInterface(Client *client);
	virtual ~FieldInterface() {}

	std::vector<uint8_t> createBoxField(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> createCylinderField(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> createSphereField(Client *callingClient, flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer
