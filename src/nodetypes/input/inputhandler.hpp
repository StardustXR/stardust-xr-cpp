#pragma once

#include "../spatial.hpp"
#include "../fields/field.hpp"
#include "distancelink.hpp"
#include <list>
#include <string>

namespace StardustXRServer {

class InputHandler : public Spatial {
public:
	InputHandler(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, Field *field, std::string callbackPath, std::string callbackMethod);
	virtual ~InputHandler();

	Field *field = nullptr;
	std::string callbackPath;
	std::string callbackMethod;

	void sendInput(std::list<DistanceLink> distanceLinks, std::vector<uint8_t> &inputData);

	std::vector<uint8_t> setCallback(flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setField(flexbuffers::Reference data, bool returnValue);
};

} // namespace StardustXRServer
