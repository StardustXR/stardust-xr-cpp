#include "inputhandler.hpp"
#include "inputmethod.hpp"
#include "../../globals.h"
#include <stardustxr/common/flatbuffers/ActionInput.hpp>
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

InputHandler::InputHandler(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, Field *field, std::string callbackPath, std::string callbackMethod) :
Spatial(client, spatialParent, position, rotation, vec3_one, true, true, false, false),
field(field) {
	this->callbackPath = callbackPath;
	this->callbackMethod = callbackMethod;

	STARDUSTXR_NODE_METHOD("setField", &InputHandler::setField);

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
	client->messenger.executeRemoteMethod(
		callbackPath.c_str(),
		callbackMethod.c_str(),
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

std::vector<uint8_t> InputHandler::setCallback(Client *callingClient, flexbuffers::Reference data, bool) {
	flexbuffers::Vector vector = data.AsVector();
	callbackPath = vector[0].AsString().str();
	callbackMethod = vector[1].AsString().str();

	return std::vector<uint8_t>();
}

std::vector<uint8_t> InputHandler::setField(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	std::string fieldPath = data.AsString().str();
	field = TypedNodeRef<Field>(client->scenegraph.findNode<Field>(fieldPath));

	if(returnValue) {
		return FLEX_SINGLE(
			FLEX_BOOL(field)
		);
	} else
		return std::vector<uint8_t>();
}

} // namespace StardustXRServer
