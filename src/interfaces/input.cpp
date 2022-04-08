#include "input.hpp"
#include "../globals.hpp"
#include "../core/client.hpp"
#include "../nodetypes/core/alias.hpp"
#include "../nodetypes/input/inputhandler.hpp"
#include "../nodetypes/input/inputmethods/pointer.hpp"
#include "../util/flex.hpp"
#include "stereokit.h"

#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/flexbuffers.h>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <mutex>
#include <string>

namespace StardustXRServer {

flatbuffers::FlatBufferBuilder InputInterface::fbb;

InputInterface::InputInterface(Client *client) : Node(client, false) {
	addChild("handler", new Node(client, false));
	addChild("method", new Node(client, false));

	STARDUSTXR_NODE_METHOD("registerInputHandler", &InputInterface::registerInputHandler)
}

std::vector<uint8_t> InputInterface::registerInputHandler(Client *callingClient, flexbuffers::Reference data, bool) {
	flexbuffers::Vector flexVec      = data.AsVector();
	std::string name                 = flexVec[0].AsString().str();
	std::string flexFieldPath        = flexVec[1].AsString().str();
	Spatial *spatialParent           = callingClient->scenegraph.findNode<Spatial>(flexVec[2].AsString().str());
	pose_t transform                 = FlexToSKPose(flexVec[3].AsTypedVector(), flexVec[4].AsTypedVector());
	std::string callbackPath         = flexVec[5].AsString().str();
	std::string callbackMethod       = flexVec[6].AsString().str();

	Field *field = client->scenegraph.findNode<Field>(flexFieldPath);

	InputHandler *handler = new InputHandler(client, spatialParent, transform, field, callbackPath, callbackMethod);
	children["handler"]->addChild(name, handler);

	return std::vector<uint8_t>();
}

void InputInterface::processInput() {
	for(auto &inputMethod : InputMethod::inputMethods.list(true)) {
		std::list<DistanceLink> distanceLinks;
		for(auto &inputHandler : InputHandler::inputHandlers.list(true)) {
			if(inputHandler->field)
				distanceLinks.push_front(inputMethod->makeDistanceLink(inputHandler));
		}
		if(distanceLinks.empty())
			continue;
		distanceLinks.sort();

		DistanceLink *closestLink = &*distanceLinks.begin();
		closestLink->handler.ptr()->sendInput(frame, distanceLinks, CreateInputData(fbb, closestLink));
	}
}

std::vector<uint8_t> InputInterface::CreateInputData(flatbuffers::FlatBufferBuilder &fbb, DistanceLink *distanceLink) {
	fbb.Clear();

	float distance = distanceLink->trueDistance;
	std::string uuid = std::to_string(distanceLink->method->id);
	StardustXR::InputDataRaw inputMethodType = distanceLink->method->type();
	flatbuffers::Offset<void> flatInputMethod = distanceLink->method->generateInput(fbb, distanceLink->handler.ptr());
	const std::vector<uint8_t> datamap = distanceLink->method->serializeDatamap();

	auto inputDataOffset = CreateInputDataDirect(fbb, uuid.c_str(), inputMethodType, flatInputMethod, distance, &datamap);
	fbb.Finish(inputDataOffset);

	return std::vector<uint8_t>(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
}

} // namespace StardustXRServer
