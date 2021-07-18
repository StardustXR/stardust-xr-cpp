#include "lifecycle.hpp"
#include "../core/client.hpp"
#include "../globals.h"
#include <stereokit.h>

namespace StardustXRServer {

StardustXR::ThreadSafeList<LifeCycleUpdateMethod> LifeCycleInterface::lifeCycleUpdateMethodList;
double LifeCycleInterface::prevFrameTime;
double LifeCycleInterface::frameTime;
double LifeCycleInterface::delta;

LifeCycleInterface::LifeCycleInterface(Client *client) : Node(client) {
	STARDUSTXR_NODE_METHOD("subscribeLogicStep", &LifeCycleInterface::subscribeLogicStep)

	prevFrameTime = sk::time_get();
	frameTime = sk::time_get();
}

void LifeCycleInterface::sendLogicStepSignals() {
	frameTime = sk::time_get();
	double delta = frameTime - prevFrameTime;

	lifeCycleUpdateMethodList.forEach([delta](uint32_t, LifeCycleUpdateMethod &method) {
		method.client->messenger.sendSignal(
			method.nodePath.c_str(),
			method.methodName.c_str(),
			[&](flexbuffers::Builder &fbb) {
				fbb.Vector([&]() {
					fbb.Double(delta);
					fbb.Double(0);
				});
			}
		);
	});

	prevFrameTime = frameTime;
}

void LifeCycleInterface::handleClientDisconnect(Client *client) {
	lifeCycleUpdateMethodList.forEach([&](uint32_t index, LifeCycleUpdateMethod &method) {
		if(method.client == client) {
			lifeCycleUpdateMethodList.erase(index);
		}
	});
}

std::vector<uint8_t> LifeCycleInterface::subscribeLogicStep(flexbuffers::Reference data, bool) {
	flexbuffers::Vector vector = data.AsVector();
	LifeCycleUpdateMethod logicStepMethod = {
		client,
		vector[0].AsString().str(),
		vector[1].AsString().str()
	};

	lifeCycleUpdateMethodList.pushBack(logicStepMethod);

	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
