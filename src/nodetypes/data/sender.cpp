#include "sender.hpp"
#include "../../core/client.hpp"
#include <stardustxr/common/flex.hpp>

#include "receiver.hpp"

using namespace sk;

namespace StardustXRServer {

Registry<PulseSender> PulseSender::senders;

PulseSender::PulseSender(Client *client, Spatial *spatialParent, pose_t pose) :
	Spatial(client, spatialParent, pose, true, true, false, false) {

	STARDUSTXR_NODE_METHOD("getReceivers", &PulseSender::getReceivers)
	STARDUSTXR_NODE_METHOD("sendData", &PulseSender::sendData)

	senders.add(this);
}
PulseSender::~PulseSender() {
	senders.remove(this);
}

std::vector<uint8_t> PulseSender::getReceivers(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	children.clear();
	std::vector<std::string> names = PulseReceiver::makeAliases(this);

	return FLEX(
		for(std::string &name : names) {
			FLEX_STRING(name);
		}
	);
}

std::vector<uint8_t> PulseSender::sendData(Client *callingClient, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec = data.AsVector();
	std::string receiverName = flexVec[0].AsString().str();
	if(children.find(receiverName) != children.end()) {
		Alias *receiverAlias = static_cast<Alias *>(children[receiverName].get());
		PulseReceiver *receiver = receiverAlias->original.ptr<PulseReceiver>();
		flexbuffers::Blob dataBlob = flexVec[1].AsBlob();
		if(receiver) {
			std::vector<uint8_t> dataBlobVector(dataBlob.data(), dataBlob.data() + dataBlob.size());
			receiver->sendData(this, dataBlobVector);
		}
	}

	return std::vector<uint8_t>();
}

}
