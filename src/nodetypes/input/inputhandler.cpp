#include "inputhandler.hpp"
#include "../../globals.h"

namespace StardustXRServer {

InputHandler::InputHandler() {

}

InputHandler::~InputHandler() {

}

void InputHandler::sendInput(std::list<InputHandler *> &handlerList, std::vector<uint8_t> &inputData) {
	messengerManager.messengers[sessionID]->executeRemoteMethod(
		callbackPath.c_str(),
		callbackMethod.c_str(),
		inputData,
		[&](flexbuffers::Reference returnData) {
			if(handlerList.begin() != handlerList.end() && !returnData.AsBool()) { // If handlerList is not empty and not captured
				InputHandler *handler = *handlerList.begin();
				handlerList.pop_front();
				handler->sendInput(handlerList, inputData);
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
