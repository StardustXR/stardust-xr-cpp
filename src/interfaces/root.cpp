#include "root.hpp"
#include "../core/client.hpp"
#include "../core/eventloop.hpp"

#include "data.hpp"
#include "drawable.hpp"
#include "field.hpp"
#include "input.hpp"
#include "item.hpp"
#include "spatial.hpp"

using namespace sk;

namespace StardustXRServer {

std::mutex RootInterface::updateMethodsMutex;
std::vector<Callback> RootInterface::updateMethods;

double RootInterface::prevFrameTime;
double RootInterface::frameTime;
double RootInterface::delta;

RootInterface::RootInterface(Client *client) : Spatial(client, nullptr, matrix_identity, false, false, false, false) {
	this->addChild("data",        new DataInterface(client));
	this->addChild("drawable",    new DrawableInterface(client));
	this->addChild("field",       new FieldInterface(client));
	this->addChild("input",       new InputInterface(client));
	this->addChild("item",        new ItemInterface(client));
	this->addChild("spatial",     new SpatialInterface(client));

	STARDUSTXR_NODE_METHOD("disconnect", &RootInterface::disconnect)
	STARDUSTXR_NODE_METHOD("applyLaunchAnchor", &RootInterface::applyLaunchAnchor)
	STARDUSTXR_NODE_METHOD("subscribeLogicStep", &RootInterface::subscribeLogicStep)

	prevFrameTime = sk::time_get();
	frameTime = sk::time_get();
}
RootInterface::~RootInterface() {
	const std::lock_guard<std::mutex> lock(updateMethodsMutex);

	updateMethods.erase(std::remove_if(updateMethods.begin(), updateMethods.end(), [&](Callback &updateMethod) {
		return updateMethod.client == client;
	}), updateMethods.end());
}

void RootInterface::sendLogicStepSignals() {
	frameTime = sk::time_get();
	double delta = frameTime - prevFrameTime;

	const std::lock_guard<std::mutex> lock(updateMethodsMutex);

	for(Callback &updateMethod : updateMethods) {
		updateMethod.signal(FLEX_ARGS(
								FLEX_DOUBLE(delta)
								FLEX_DOUBLE(0)
							));
	}

	prevFrameTime = frameTime;
}

std::vector<uint8_t> RootInterface::applyLaunchAnchor(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	uint32_t anchorID = data.AsUInt32();
	setTransformMatrix(anchors[anchorID]);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> RootInterface::subscribeLogicStep(Client *callingClient, flexbuffers::Reference data, bool) {
	const std::lock_guard<std::mutex> lock(updateMethodsMutex);

	flexbuffers::Vector vector = data.AsVector();
	updateMethods.push_back(Callback {
		callingClient,
		vector[0].AsString().str(),
		vector[1].AsString().str()
	});

	return std::vector<uint8_t>();
}
std::vector<uint8_t> RootInterface::disconnect(Client *callingClient, flexbuffers::Reference data, bool) {
	callingClient->eventLoop->closeConnection(callingClient);
	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
