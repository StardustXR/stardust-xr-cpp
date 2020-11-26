#include "lifecycle.hpp"
#include "../scenegraph.hpp"
#include "../../globals.h"
#include <stereokit.h>

namespace StardustXRServer {

LifeCycleInterface::LifeCycleInterface() {
	STARDUSTXR_NODE_METHOD("subscribeLogicStep", &LifeCycleInterface::subscribeLogicStep)

	prevFrameTime = sk::time_get();
	frameTime = sk::time_get();
}

void LifeCycleInterface::sendLogicStepSignals() {
	frameTime = sk::time_get();
	double delta = frameTime - prevFrameTime;

	for(const auto &method : lifeCycleUpdateMethodList) {
		messengerManager.messengers[method.sessionID]->sendSignal(
			method.nodePath.c_str(),
			method.methodName.c_str(),
			[&](flexbuffers::Builder &fbb) {
				fbb.Vector([&]() {
					fbb.Double(delta);
					fbb.Double(0);
				});
			}
		);
	}

	prevFrameTime = frameTime;
}

void LifeCycleInterface::handleMessengerDeletion(uint sessionID) {
	for(auto i = lifeCycleUpdateMethodList.cbegin(); i != lifeCycleUpdateMethodList.end(); ++i) {
		if(i->sessionID == sessionID) {
			lifeCycleUpdateMethodList.erase(i--);
		}
	}
}

std::vector<uint8_t> LifeCycleInterface::subscribeLogicStep(uint sessionID, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector vector = data.AsVector();
	LifeCycleUpdateMethod logicStepMethod = {
		sessionID,
		vector[0].AsString().str(),
		vector[1].AsString().str()
	};

	lifeCycleUpdateMethodList.push_back(logicStepMethod);

	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
