#include "input.hpp"
#include "../globals.h"
#include "../core/client.hpp"
#include "../nodetypes/input/inputmethods/pointer.hpp"
#include <stardustxr/common/flatbuffers/Input.hpp>
#include "nodetypes/core/alias.hpp"
#include "nodetypes/input/inputhandler.hpp"
#include "stereokit.h"
#include <flatbuffers/flexbuffers.h>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <mutex>
#include <stardustxr/common/flex.hpp>
#include <string>

namespace StardustXRServer {

std::mutex InputInterface::inputVectorsMutex;
std::vector<InputMethod *> InputInterface::inputMethods;
std::vector<InputHandler *> InputInterface::inputHandlers;

flatbuffers::FlatBufferBuilder InputInterface::fbb;

InputInterface::InputInterface(Client *client) : Node(client, false) {
	addChild("handler", new Node(client, false));
	addChild("method", new Node(client, false));
	addChild("global_handler", new Node(client, false));

	STARDUSTXR_NODE_METHOD("getInputHandlers", &InputInterface::getInputHandlers)
	STARDUSTXR_NODE_METHOD("registerInputHandler", &InputInterface::registerInputHandler)
}
InputInterface::~InputInterface() {
	const std::lock_guard<std::mutex> lock(inputVectorsMutex);
	
	inputMethods.erase(std::remove_if(inputMethods.begin(), inputMethods.end(), [this](InputMethod *method) {
		return method->client == client;
	}), inputMethods.end());
	inputHandlers.erase(std::remove_if(inputHandlers.begin(), inputHandlers.end(), [this](InputHandler *handler) {
		return handler->client == client;
	}), inputHandlers.end());
}

std::vector<uint8_t> InputInterface::getInputHandlers(flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec = data.AsVector();

	//If the spacePath doesn't exist, it must be world space
	Spatial *space = client->scenegraph.findNode<Spatial>(flexVec[0].AsString().str());
	bool excludeSelf = flexVec[1].AsBool();

	const std::lock_guard<std::mutex> lock(inputVectorsMutex);
	return StardustXR::FlexbufferFromArguments([&](flexbuffers::Builder &fbb) {
		fbb.Vector([&]() {
			children["global_handler"]->children.clear();
			for(InputHandler *handler : inputHandlers) {
				if(excludeSelf == false || handler->client != this->client) {
					fbb.Vector([&] {
						std::ostringstream stringStream;
						std::size_t hash = std::hash<std::string>{}(handler->name);
						stringStream << std::uintptr_t((std::uintptr_t) handler->client ^ (std::uintptr_t) hash);
						children["global_handler"]->addChild(stringStream.str(), new Alias(client, handler, {"getActions", "runAction"}));
						fbb.String(stringStream.str());

						fbb.TypedVector([&] {
							sk::vec3 position = handler->localToSpacePoint(space, vec3_zero);
							fbb.Float(position.x);
							fbb.Float(position.y);
							fbb.Float(position.z);
						});
					});
				}
			}
		});
	});
}

std::vector<uint8_t> InputInterface::registerInputHandler(flexbuffers::Reference data, bool) {
	const std::lock_guard<std::mutex> lock(inputVectorsMutex);

	flexbuffers::Vector flexVec      = data.AsVector();
	std::string name                 = flexVec[0].AsString().str();
	std::string flexFieldPath        = flexVec[1].AsString().str();
	Spatial *spatialParent           = this->client->scenegraph.findNode<Spatial>(flexVec[2].AsString().str());
	flexbuffers::TypedVector flexPos = flexVec[3].AsTypedVector();
	flexbuffers::TypedVector flexRot = flexVec[4].AsTypedVector();
	std::string callbackPath         = flexVec[5].AsString().str();
	std::string callbackMethod       = flexVec[6].AsString().str();

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
	Field *field = client->scenegraph.findNode<Field>(flexFieldPath);

	InputHandler *handler = new InputHandler(client, spatialParent, pos, rot, field, callbackPath, callbackMethod);
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
			if(inputHandler->field == nullptr)
				continue;
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
			frame,
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
