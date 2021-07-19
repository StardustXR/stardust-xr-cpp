#include "input.hpp"
#include "../globals.h"
#include "../nodetypes/input/inputmethods/pointer.hpp"
#include "../nodetypes/input/inputmethods/flatbuffers/Input_generated.h"
#include "../core/client.hpp"
#include <mutex>

namespace StardustXRServer {

std::mutex InputInterface::inputVectorsMutex;
std::vector<InputMethod *> InputInterface::inputMethods;
std::vector<InputHandler *> InputInterface::inputHandlers;

flatbuffers::FlatBufferBuilder InputInterface::fbb;

InputInterface::InputInterface(Client *client) : Node(client) {
	children.emplace("handler", new Node(client));
	children["handler"]->parent = this;

	children.emplace("method", new Node(client));
	children["method"]->parent = this;

	STARDUSTXR_NODE_METHOD("registerInputHandler", &InputInterface::registerInputHandler)
}
InputInterface::~InputInterface() {}

void InputInterface::handleClientDisconnect(Client *client) {
	const std::lock_guard<std::mutex> lock(inputVectorsMutex);
	
	inputMethods.erase(std::remove_if(inputMethods.begin(), inputMethods.end(), [&client](InputMethod *method) {
		return method->client == client;
	}), inputMethods.end());
	inputHandlers.erase(std::remove_if(inputHandlers.begin(), inputHandlers.end(), [&client](InputHandler *handler) {
		return handler->client == client;
	}), inputHandlers.end());
}

std::vector<uint8_t> InputInterface::registerInputHandler(flexbuffers::Reference data, bool) {
	const std::lock_guard<std::mutex> lock(inputVectorsMutex);

	flexbuffers::Vector flexVec      = data.AsVector();
	std::string name                 = flexVec[0].AsString().str();
	std::string flexFieldPath        = flexVec[1].AsString().str();
	flexbuffers::TypedVector flexPos = flexVec[2].AsTypedVector();
	flexbuffers::TypedVector flexRot = flexVec[3].AsTypedVector();
	std::string callbackPath         = flexVec[4].AsString().str();
	std::string callbackMethod       = flexVec[5].AsString().str();

	sk::vec3 pos = {
		flexPos[0].AsFloat(), 
		flexPos[1].AsFloat(),
		flexPos[2].AsFloat()
	};
	sk::quat rot = {
		flexRot[0].AsFloat(), 
		flexRot[1].AsFloat(),
		flexRot[2].AsFloat(),
		flexRot[3].AsFloat()
	};
	Field *field = client->scenegraph.findNode<Field *>(flexFieldPath);

	InputHandler *handler = new InputHandler(client, nullptr, pos, rot, field, callbackPath, callbackMethod);
	children["handler"]->addChild(name, handler);
	inputHandlers.push_back(handler);

	return std::vector<uint8_t>();
}

void InputInterface::processInput() {
	const std::lock_guard<std::mutex> lock(inputVectorsMutex);

	const uint32_t inputMethodCount = inputMethods.size();
	const uint32_t inputHandlerCount = inputHandlers.size();

	if(inputMethodCount == 0 || inputHandlerCount == 0)
		return;

	for(auto &inputMethod : inputMethods) {
		std::list<DistanceLink> distanceLinks;
		for(auto &inputHandler : inputHandlers) {
			distanceLinks.push_front(DistanceLink {
				inputMethod,
				inputMethod->distanceTo(inputHandler),
				inputHandler
			});
		}
		distanceLinks.sort();

		std::vector<uint8_t> inputData = CreateInputData(
			fbb,
			distanceLinks.begin()->method,
			distanceLinks.begin()->handler
		);

		distanceLinks.begin()->handler->sendInput(
			distanceLinks,
			inputData
		);
	}
}

void InputInterface::addInputMethod(InputMethod *method) {
	const std::lock_guard<std::mutex> lock(inputVectorsMutex);

	inputMethods.push_back(method);
}

void InputInterface::addInputHandler(InputHandler *handler) {
	const std::lock_guard<std::mutex> lock(inputVectorsMutex);

	inputHandlers.push_back(handler);
}

std::vector<uint8_t> InputInterface::CreateInputData(flatbuffers::FlatBufferBuilder &fbb, InputMethod* inputMethod, InputHandler *inputHandler) {
	float distance = inputMethod->distanceTo(inputHandler);
	StardustXR::InputDataRaw inputMethodType = inputMethod->type();
	flatbuffers::Offset<void> flatInputMethod = inputMethod->generateInput(fbb, inputHandler);
	const std::vector<uint8_t> datamap = inputMethod->serializeDatamap();

	auto inputDataOffset = CreateInputDataDirect(fbb, inputMethodType, flatInputMethod, distance, &datamap);
	fbb.Finish(inputDataOffset);

	std::vector<uint8_t> data;
	data.resize(fbb.GetSize());
	memcpy(data.data(), fbb.GetBufferPointer(), fbb.GetSize());

	fbb.Clear();

	return data;
}

} // namespace StardustXRServer
