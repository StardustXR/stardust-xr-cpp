#include "input.hpp"
#include "../../globals.h"
#include "../../nodetypes/input/inputmethods/pointer.hpp"
#include "../../nodetypes/input/inputmethods/flatbuffers/Input_generated.h"

namespace StardustXRServer {

InputInterface::InputInterface() {
	children["handler"] = new Node();
	children["handler"]->parent = this;

	children["method"] = new Node();
	children["method"]->parent = this;

	STARDUSTXR_NODE_METHOD("registerInputHandler", &InputInterface::registerInputHandler)
}
InputInterface::~InputInterface() {}

void InputInterface::handleMessengerDeletion(uint sessionID) {
	//Since all the methods/handlers that are to be deleted shift every other index, the proper index to erase from is the # of items not deleted.
	uint32_t methodsToKeep = 0;
	inputMethods.forEach([&](uint32_t, InputMethod *method) {
		if(method->sessionID == sessionID)
			inputMethods.erase(methodsToKeep);
		else
			methodsToKeep++;
	});
	uint32_t handlersToKeep = 0;
	inputHandlers.forEach([&](uint32_t, InputHandler *handler) {
		if(handler->sessionID == sessionID)
			inputHandlers.erase(handlersToKeep);
		else
			handlersToKeep++;
	});
	inputMethods.done();
	inputHandlers.done();
}

std::vector<uint8_t> InputInterface::registerInputHandler(uint sessionID, flexbuffers::Reference data, bool) {
	flexbuffers::Vector vec      = data.AsVector();
	std::string name             = vec[0].AsString().str();
	std::string field            = vec[1].AsString().str();
	flexbuffers::TypedVector pos = vec[2].AsTypedVector();
	flexbuffers::TypedVector rot = vec[3].AsTypedVector();
	std::string callbackPath     = vec[4].AsString().str();
	std::string callbackMethod   = vec[5].AsString().str();

	InputHandler *handler               = new InputHandler();
	handler->ready                      = false;
	handler->parent                     = children["handler"];
	children["handler"]->children[name] = handler;
	handler->sessionID                  = sessionID;
	handler->field                      = dynamic_cast<Field *>(scenegraph.findNode(field));
	handler->position                   = { pos[0].AsFloat(), pos[1].AsFloat(), pos[2].AsFloat() };
	handler->rotation                   = { rot[0].AsFloat(), rot[1].AsFloat(), rot[2].AsFloat(), rot[3].AsFloat() };
	handler->callbackPath               = callbackPath;
	handler->callbackMethod             = callbackMethod;
	handler->transformDirty();
	handler->ready                      = true;

	inputHandlers.pushBack(handler);
	inputHandlers.done();

	return std::vector<uint8_t>();
}

void InputInterface::processInput() {
	const uint32_t inputMethodCount = inputMethods.length();
	const uint32_t inputHandlerCount = inputHandlers.length();

	if(inputMethodCount == 0 || inputHandlerCount == 0)
		goto RETURN;

	for(uint32_t i=0; i<inputMethodCount; ++i) {
		std::list<DistanceLink> distanceLinks;
		for(uint32_t j=0; j<inputHandlerCount; ++j) {
			distanceLinks.push_front({
				inputMethods[i],
				inputMethods[i]->distanceTo(inputHandlers[j]),
				inputHandlers[j]
			});
		}
		distanceLinks.sort();

		vector<uint8_t> inputData = CreateInputData(
			fbb,
			distanceLinks.begin()->method,
			distanceLinks.begin()->handler
		);

		distanceLinks.begin()->handler->sendInput(
			distanceLinks,
			inputData
		);
	}

	RETURN:
	inputMethods.done();
	inputHandlers.done();
}

std::vector<uint8_t> InputInterface::CreateInputData(flatbuffers::FlatBufferBuilder &fbb, InputMethod* inputMethod, InputHandler *inputHandler) {
	float distance = inputMethod->distanceTo(inputHandler);
	StardustXR::InputDataRaw inputMethodType = inputMethod->type();
	flatbuffers::Offset<void> flatInputMethod = inputMethod->generateInput(fbb, inputHandler);
	const vector<uint8_t> datamap = inputMethod->serializeDatamap();

	auto inputDataOffset = CreateInputDataDirect(fbb, inputMethodType, flatInputMethod, distance, &datamap);
	fbb.Finish(inputDataOffset);

	std::vector<uint8_t> data;
	data.resize(fbb.GetSize());
	memcpy(data.data(), fbb.GetBufferPointer(), fbb.GetSize());

	fbb.Clear();

	return data;
}

} // namespace StardustXRServer
