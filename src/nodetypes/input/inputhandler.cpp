#include "inputhandler.hpp"
#include "inputmethod.hpp"
#include "../../globals.h"
#include "inputmethods/flatbuffers/Input_generated.h"

#include "../../core/client.hpp"
#include <stereokit.h>

namespace StardustXRServer {

InputHandler::InputHandler(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, Field *field, std::string callbackPath, std::string callbackMethod) : Spatial(client, spatialParent, position, rotation, vec3_one, true, true, false) {
	this->field = field;
	this->callbackPath = callbackPath;
	this->callbackMethod = callbackMethod;
}

InputHandler::~InputHandler() {}

void InputHandler::sendInput(uint64_t oldFrame, std::list<DistanceLink> distanceLinks, std::vector<uint8_t> &inputData) {
	if(oldFrame < frame)
		return;

	distanceLinks.pop_front();
	client->messenger.executeRemoteMethod(
		callbackPath.c_str(),
		callbackMethod.c_str(),
		[&](flexbuffers::Builder &fbb) {
			fbb.Blob(inputData);
		},
		[oldFrame, distanceLinks, inputData](flexbuffers::Reference returnData) {
			if(distanceLinks.begin() != distanceLinks.end() && !returnData.AsBool()) { // If handlerList is not empty and not captured
				std::vector<uint8_t> inputDataCopy = inputData;
				InputData *parsedInputData = GetMutableInputData(inputDataCopy.data());

				InputMethod *method = distanceLinks.begin()->method;
				InputHandler *handler = distanceLinks.begin()->handler;
				method->updateInput(parsedInputData, handler);

				handler->sendInput(oldFrame, distanceLinks, inputDataCopy);
			}
		}
	);
}

std::vector<uint8_t> InputHandler::setCallback(flexbuffers::Reference data, bool) {
	flexbuffers::Vector vector = data.AsVector();
	callbackPath = vector[0].AsString().str();
	callbackMethod = vector[1].AsString().str();

	return std::vector<uint8_t>();
}

std::vector<uint8_t> InputHandler::setField(flexbuffers::Reference data, bool returnValue) {
	std::string fieldPath = data.AsString().str();
	field = client->scenegraph.findNode<Field *>(fieldPath);

	if(returnValue) {
		return FLEX_SINGLE(
			FLEX_BOOL(field == nullptr)
		);
	} else
		return std::vector<uint8_t>();
}

} // namespace StardustXRServer
