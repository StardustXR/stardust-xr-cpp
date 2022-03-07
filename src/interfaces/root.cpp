#include "root.hpp"
#include "../core/client.hpp"

#include "data.hpp"
#include "drawable.hpp"
#include "field.hpp"
#include "input.hpp"
#include "item.hpp"
#include "spatial.hpp"

using namespace sk;

namespace StardustXRServer {

std::mutex RootInterface::updateMethodsMutex;
std::vector<LifeCycleUpdateMethod> RootInterface::updateMethods;

double RootInterface::prevFrameTime;
double RootInterface::frameTime;
double RootInterface::delta;

RootInterface::RootInterface(Client *client) : Spatial(client, nullptr, vec3_zero, quat_identity, vec3_one, false, false, false, false) {
	this->addChild("data",        new DataInterface(client));
	this->addChild("drawable",    new DrawableInterface(client));
	this->addChild("field",       new FieldInterface(client));
	this->addChild("input",       new InputInterface(client));
	this->addChild("item",        new ItemInterface(client));
	this->addChild("spatial",     new SpatialInterface(client));

	STARDUSTXR_NODE_METHOD("disconnect", &RootInterface::disconnect)
	STARDUSTXR_NODE_METHOD("subscribeLogicStep", &RootInterface::subscribeLogicStep)

	prevFrameTime = sk::time_get();
	frameTime = sk::time_get();
}
RootInterface::~RootInterface() {
	const std::lock_guard<std::mutex> lock(updateMethodsMutex);

	updateMethods.erase(std::remove_if(updateMethods.begin(), updateMethods.end(), [&](LifeCycleUpdateMethod &updateMethod) {
		return updateMethod.client == client;
	}), updateMethods.end());
}

void RootInterface::sendLogicStepSignals() {
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

std::vector<uint8_t> RootInterface::subscribeLogicStep(Client *callingClient, flexbuffers::Reference data, bool) {
	const std::lock_guard<std::mutex> lock(updateMethodsMutex);

	flexbuffers::Vector vector = data.AsVector();
	updateMethods.push_back(LifeCycleUpdateMethod {
		callingClient,
		vector[0].AsString().str(),
		vector[1].AsString().str()
	});

	return std::vector<uint8_t>();
}
std::vector<uint8_t> RootInterface::disconnect(Client *callingClient, flexbuffers::Reference data, bool) {
	callingClient->disconnected = true;
	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
