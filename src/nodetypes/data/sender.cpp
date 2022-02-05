#include "sender.hpp"
#include "../../core/client.hpp"
#include <stardustxr/common/flex.hpp>

#include "receiver.hpp"

namespace StardustXRServer {

std::mutex NonSpatialSender::sendersMutex;
std::vector<NonSpatialSender *> NonSpatialSender::senders;

NonSpatialSender::NonSpatialSender(Client *client, Spatial *spatialParent, vec3 position, quat rotation) :
	Spatial(client, spatialParent, position, rotation, vec3_one, true, true, false, false) {

	STARDUSTXR_NODE_METHOD("getReceivers", &NonSpatialSender::getReceivers)
	STARDUSTXR_NODE_METHOD("sendData", &NonSpatialSender::sendData)

	const std::lock_guard<std::mutex> lock(sendersMutex);
	senders.push_back(this);
}

NonSpatialSender::~NonSpatialSender() {
	const std::lock_guard<std::mutex> lock(sendersMutex);
	senders.erase(std::remove(senders.begin(), senders.end(), this));
}

std::vector<uint8_t> NonSpatialSender::getReceivers(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	children.clear();
	std::vector<std::string> names = NonSpatialReceiver::makeAliases(this);

	return FLEX(
		for(std::string &name : names) {
			FLEX_STRING(name);
		}
	);
}

std::vector<uint8_t> NonSpatialSender::sendData(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec = data.AsVector();
	std::string receiverName = flexVec[0].AsString().str();
	if(children.find(receiverName) != children.end()) {
		Alias *receiverAlias = static_cast<Alias *>(children[receiverName].get());
		NonSpatialReceiver *receiver = receiverAlias->original.ptr<NonSpatialReceiver>();
		flexbuffers::Blob dataBlob = flexVec[1].AsBlob();
		if(receiver)
			receiver->sendData(this, dataBlob.data(), dataBlob.size());
	}

	return std::vector<uint8_t>();
}

}
