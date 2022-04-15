#include "inputhandler.hpp"
#include "inputmethod.hpp"
#include "../../globals.h"
#include <stardustxr/common/flatbuffers/Input.hpp>
#include <stardustxr/common/flex.hpp>
#include <vector>

#include "../../core/client.hpp"
#include "../../interfaces/input.hpp"
#include "nodetypes/core/node.hpp"
#include <flatbuffers/flexbuffers.h>
#include <stereokit.h>

namespace StardustXRServer {

Registry<InputHandler> InputHandler::inputHandlers;

InputHandler::InputHandler(Client *client, Spatial *spatialParent, pose_t transform, Field *field, std::string callbackPath, std::string callbackMethod) :
Spatial(client, spatialParent, transform, true, true, false, false),
field(field),
callback({client, callbackPath, callbackMethod}) {
	inputHandlers.add(this);
}
InputHandler::~InputHandler() {
	inputHandlers.remove(this);
}

void InputHandler::sendInput(uint64_t oldFrame, std::list<DistanceLink> distanceLinks, const std::vector<uint8_t> &inputData) {
	if(oldFrame < frame)
		return;

	distanceLinks.pop_front();

	if(!enabled) {
		sendInputCallback(oldFrame, distanceLinks, inputData, false);
		return;
	}
	callback.executeMethod(
		[&](flexbuffers::Builder &fbb) {
			fbb.Blob(inputData);
		},
		[this, oldFrame, distanceLinks, inputData](flexbuffers::Reference returnData) {
			sendInputCallback(oldFrame, distanceLinks, inputData, returnData.AsBool());
		}
	);
}

void InputHandler::sendInputCallback(uint64_t oldFrame, std::list<DistanceLink> distanceLinks, const std::vector<uint8_t> &inputData, bool capture) {
	if(distanceLinks.begin() != distanceLinks.end() && !capture) { // If handlerList is not empty and not captured
		std::vector<uint8_t> inputDataCopy = inputData;
		InputData *parsedInputData = GetMutableInputData(inputDataCopy.data());

		DistanceLink distanceLink = *distanceLinks.begin();
		if(distanceLink.method && distanceLink.handler) {
			distanceLink.method.ptr()->updateInput(parsedInputData, distanceLink.handler.ptr());

			distanceLink.handler.ptr()->sendInput(oldFrame, distanceLinks, inputDataCopy);
		}
	}
}

} // namespace StardustXRServer
