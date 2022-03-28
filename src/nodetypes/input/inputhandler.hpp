#pragma once

#include "../core/callback.hpp"
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
	InputHandler(Client *client, Spatial *spatialParent, sk::pose_t transform, Field *field, std::string callbackPath, std::string callbackMethod);
	static Registry<InputHandler> inputHandlers;
	~InputHandler();

	TypedNodeRef<Field> field;
	Callback callback;

	void sendInput        (uint64_t oldFrame, std::list<DistanceLink> distanceLinks, std::vector<uint8_t> const &inputData);
	void sendInputCallback(uint64_t oldFrame, std::list<DistanceLink> distanceLinks, std::vector<uint8_t> const &inputData, bool capture);
protected:
	flatbuffers::FlatBufferBuilder flbb;
};

} // namespace StardustXRServer
