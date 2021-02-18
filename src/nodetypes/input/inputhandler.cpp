#include "inputhandler.hpp"
#include "inputmethod.hpp"
#include "../../globals.h"
#include "inputmethods/flatbuffers/Input_generated.h"

namespace StardustXRServer {

InputHandler::InputHandler() {

}

InputHandler::~InputHandler() {

}

void InputHandler::sendInput(std::list<DistanceLink> distanceLinks, std::vector<uint8_t> &inputData) {
	distanceLinks.pop_front();
	messengerManager.messengers[sessionID]->executeRemoteMethod(
		callbackPath.c_str(),
		callbackMethod.c_str(),
		[&](flexbuffers::Builder &fbb) {
			fbb.Blob(inputData);
		},
		[distanceLinks, inputData](flexbuffers::Reference returnData) {
			if(distanceLinks.begin() != distanceLinks.end() && !returnData.AsBool()) { // If handlerList is not empty and not captured
				std::vector<uint8_t> inputDataCopy = inputData;
				InputData *parsedInputData = GetMutableInputData(inputDataCopy.data());

				InputMethod *method = distanceLinks.begin()->method;
				InputHandler *handler = distanceLinks.begin()->handler;
				method->updateInput(parsedInputData, nullptr);

				handler->sendInput(distanceLinks, inputDataCopy);
			}
		}
	);
}

std::vector<uint8_t> InputHandler::setCallback(uint sessionID, flexbuffers::Reference data, bool) {
	if(sessionID != this->sessionID)
		return std::vector<uint8_t>();

	flexbuffers::Vector vector = data.AsVector();
	callbackPath = vector[0].AsString().str();
	callbackMethod = vector[1].AsString().str();

	return std::vector<uint8_t>();
}

std::vector<uint8_t> InputHandler::setField(uint sessionID, flexbuffers::Reference data, bool returnValue) {
	if(sessionID != this->sessionID)
		return std::vector<uint8_t>();

	std::string fieldPath = data.AsString().str();
	field = dynamic_cast<Field *>(scenegraph.findNode(fieldPath));

	if(returnValue) {
		return FlexbufferFromArguments([&](flexbuffers::Builder &fbb) {
			fbb.Bool(field == nullptr);
		});
	} else
		return std::vector<uint8_t>();
}

} // namespace StardustXRServer
