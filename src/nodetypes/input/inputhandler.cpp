#include "inputhandler.hpp"
#include "../../globals.h"

namespace StardustXRServer {

InputHandler::InputHandler() {

}

InputHandler::~InputHandler() {

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
