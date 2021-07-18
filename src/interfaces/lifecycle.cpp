#include "lifecycle.hpp"
#include "../core/client.hpp"
#include <stereokit.h>

using namespace StardustXR;
namespace StardustXRServer {

std::mutex LifeCycleInterface::updateMethodsMutex;
std::vector<LifeCycleUpdateMethod> LifeCycleInterface::updateMethods;

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

	const std::lock_guard<std::mutex> lock(updateMethodsMutex);

	for(LifeCycleUpdateMethod &updateMethod : updateMethods) {
		updateMethod.client->messenger.sendSignal(
			updateMethod.nodePath.c_str(),
			updateMethod.methodName.c_str(),
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

void LifeCycleInterface::handleClientDisconnect(Client *client) {
	const std::lock_guard<std::mutex> lock(updateMethodsMutex);

	updateMethods.erase(std::remove_if(updateMethods.begin(), updateMethods.end(), [&client](LifeCycleUpdateMethod &updateMethod) {
		return updateMethod.client == client;
	}), updateMethods.end());
}

std::vector<uint8_t> LifeCycleInterface::subscribeLogicStep(flexbuffers::Reference data, bool) {
	const std::lock_guard<std::mutex> lock(updateMethodsMutex);

	flexbuffers::Vector vector = data.AsVector();
	updateMethods.push_back(LifeCycleUpdateMethod {
		client,
		vector[0].AsString().str(),
		vector[1].AsString().str()
	});

	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
