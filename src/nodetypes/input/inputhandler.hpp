#pragma once

#include "../fields/field.hpp"
#include "../spatial/spatial.hpp"
#include "distancelink.hpp"
#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/flexbuffers.h>
#include <list>
#include <string>

namespace StardustXRServer {

class InputHandler : public Spatial {
public:
	InputHandler(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, Field *field, std::string callbackPath, std::string callbackMethod);
	static Registry<InputHandler> inputHandlers;
	~InputHandler();

	TypedNodeRef<Field> field;
	std::string callbackPath;
	std::string callbackMethod;

	void sendInput        (uint64_t oldFrame, std::list<DistanceLink> distanceLinks, std::vector<uint8_t> const &inputData);
	void sendInputCallback(uint64_t oldFrame, std::list<DistanceLink> distanceLinks, std::vector<uint8_t> const &inputData, bool capture);

	std::vector<uint8_t> setCallback(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setField(Client *callingClient, flexbuffers::Reference data, bool returnValue);
protected:
	flatbuffers::FlatBufferBuilder flbb;
};

} // namespace StardustXRServer
