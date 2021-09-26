#include "inputhandler.hpp"
#include "inputmethod.hpp"
#include "../../globals.h"
#include <stardustxr/common/flatbuffers/ActionInput.hpp>
#include <stardustxr/common/flatbuffers/Input.hpp>
#include <stardustxr/common/flex.hpp>
#include <vector>

#include "../../core/client.hpp"
#include "nodetypes/node.hpp"
#include <flatbuffers/flexbuffers.h>
#include <stereokit.h>

namespace StardustXRServer {

InputHandler::InputHandler(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, Field *field, std::string callbackPath, std::string callbackMethod) :
Spatial(client, spatialParent, position, rotation, vec3_one, true, true, false) {
	this->field = field;
	this->callbackPath = callbackPath;
	this->callbackMethod = callbackMethod;

	STARDUSTXR_NODE_METHOD("setField", &InputHandler::setField);
	STARDUSTXR_NODE_METHOD("setActions", &InputHandler::setActions);
	STARDUSTXR_NODE_METHOD("getActions", &InputHandler::getActions);
	STARDUSTXR_NODE_METHOD("runAction", &InputHandler::runAction);
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
	field = client->scenegraph.findNode<Field>(fieldPath);

	if(returnValue) {
		return FLEX_SINGLE(
			FLEX_BOOL(field == nullptr)
		);
	} else
		return std::vector<uint8_t>();
}


std::vector<uint8_t> InputHandler::setActions(flexbuffers::Reference data, bool returnValue) {
	flexbuffers::TypedVector flexVec = data.AsTypedVector();
	actions.clear();
	for(uint i=0; i<flexVec.size(); ++i) {
		actions.push_back(flexVec[i].AsString().str());
	}
	return std::vector<uint8_t>();
}
std::vector<uint8_t> InputHandler::getActions(flexbuffers::Reference data, bool returnValue) {
	return StardustXR::FlexbufferFromArguments([&](flexbuffers::Builder &fbb) {
		fbb.TypedVector([&](){
			for(std::string &action : actions) {
				fbb.String(action);
			}
		});
	});
}
std::vector<uint8_t> InputHandler::runAction(flexbuffers::Reference data, bool returnValue) {
	std::string actionString = data.AsString().str();
	if(std::find(actions.begin(), actions.end(), actionString) == actions.end()) {
		return std::vector<uint8_t>();
	}

	auto action = StardustXR::CreateActionDirect(flbb, "test").Union();
	std::vector<uint8_t> flex = StardustXR::FlexbufferFromArguments([&](flexbuffers::Builder &fbb) {
		fbb.Map([&]() {});
	});
	auto input = StardustXR::CreateInputDataDirect(flbb, StardustXR::InputDataRaw_Action, action, 0.0f, &flex);
	flbb.Finish(input);
	std::vector<uint8_t> actionData;
	actionData.resize(flbb.GetSize());
	memcpy(actionData.data(), flbb.GetBufferPointer(), flbb.GetSize());
	flbb.Clear();
	
	client->messenger.sendSignal(
		callbackPath.c_str(),
		callbackMethod.c_str(),
		[&](flexbuffers::Builder &fbb) {
			fbb.Blob(actionData);
		}
	);

	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
