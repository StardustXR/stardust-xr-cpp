#pragma once

#include "../spatialnode.hpp"
#include "../fields/field.hpp"
#include "distancelink.hpp"
#include <list>

namespace StardustXRServer {

class InputHandler : public SpatialNode {
public:
	InputHandler(Client *client);
	virtual ~InputHandler();

	Field *field = nullptr;
	std::string callbackPath;
	std::string callbackMethod;

	void sendInput(std::list<DistanceLink> distanceLinks, std::vector<uint8_t> &inputData);

	std::vector<uint8_t> setCallback(uint sessionID, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setField(uint sessionID, flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer
