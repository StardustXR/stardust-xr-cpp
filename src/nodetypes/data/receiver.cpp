#include "receiver.hpp"
#include "../../core/client.hpp"
#include "../core/alias.hpp"

#include "sender.hpp"

namespace StardustXRServer {

std::mutex NonSpatialReceiver::receiversMutex;
std::vector<NonSpatialReceiver *> NonSpatialReceiver::receivers;

NonSpatialReceiver::NonSpatialReceiver(Client *client, Spatial *spatialParent, vec3 position, quat rotation, Field *field, std::string callbackPath, std::string callbackMethod) :
	Spatial(client, spatialParent, position, rotation, vec3_one, true, true, false, false),
	field(field) {

	this->callbackPath = callbackPath;
	this->callbackMethod = callbackMethod;

	STARDUSTXR_NODE_METHOD("getMask", &NonSpatialReceiver::getMask)
	STARDUSTXR_NODE_METHOD("setMask", &NonSpatialReceiver::setMask)

	const std::lock_guard<std::mutex> lock(receiversMutex);
	receivers.push_back(this);
}

NonSpatialReceiver::~NonSpatialReceiver() {
	const std::lock_guard<std::mutex> lock(receiversMutex);
	receivers.erase(std::remove(receivers.begin(), receivers.end(), this));
}

std::vector<std::string> NonSpatialReceiver::makeAliases(Node *parent) {
	const std::lock_guard lock(receiversMutex);
	std::vector<std::string> aliasNames;
	for(auto const receiver : receivers) {
		std::string id = std::to_string(receiver->id);
		Alias *alias = new Alias(parent->client, receiver, {"getTransform", "getMask"});
		Alias *field = new Alias(parent->client, receiver->field, {"distance", "normal", "closestPoint"});
		parent->addChild(id, alias);
		alias->addChild("field", field);
		aliasNames.push_back(id);
	}
	return aliasNames;
}

void NonSpatialReceiver::sendData(NonSpatialSender *sender, const uint8_t *data, size_t dataSize) {
	flexbuffers::Map dataMap = flexbuffers::GetRoot(data, dataSize).AsMap();
	for(size_t i=0; i<maskMap.Keys().size(); ++i) {
		std::string key = maskMap.Keys()[i].AsKey();
		flexbuffers::Reference maskValue = maskMap[key];
		flexbuffers::Reference dataValue = dataMap[key];

		if(maskValue.GetType() != dataValue.GetType())
			return;
		if(maskValue.ToString() != dataValue.ToString())
			return;
	}

	client->messenger.sendSignal(
		callbackPath.c_str(),
		callbackMethod.c_str(),
		[&](flexbuffers::Builder &fbb) {
			fbb.Vector([&] {
				fbb.String(std::to_string(this->id));
				fbb.Blob(data, dataSize);
			});
		}
	);
}

std::vector<uint8_t> NonSpatialReceiver::getMask(flexbuffers::Reference data, bool returnValue) {
	return maskBinary;
}
std::vector<uint8_t> NonSpatialReceiver::setMask(flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Blob maskBlob = data.AsBlob();
	this->maskBinary = std::vector<uint8_t>(maskBlob.data(), maskBlob.data() + maskBlob.size());
	this->maskMap = flexbuffers::GetRoot(this->maskBinary).AsMap();

	return std::vector<uint8_t>();
}

}
