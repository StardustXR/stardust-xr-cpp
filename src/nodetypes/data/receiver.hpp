#pragma once

#include "../core/typednoderef.hpp"
#include "../core/callback.hpp"
#include "../spatial/spatial.hpp"
#include "../fields/field.hpp"

namespace StardustXRServer {

class NonSpatialSender;

class NonSpatialReceiver : public Spatial {
public:
	NonSpatialReceiver(Client *client, Spatial *spatialParent, sk::pose_t pose, Field *field, std::string callbackPath, std::string callbackMethod);
	~NonSpatialReceiver();

	TypedNodeRef<Field> field;
	Callback eventCallback;

	static Registry<NonSpatialReceiver> receivers;
	static std::vector<std::string> makeAliases(Node *parent);

	void sendData(NonSpatialSender *sender, const std::vector<uint8_t> &data);

	std::vector<uint8_t> getMask(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setMask(Client *callingClient, flexbuffers::Reference data, bool returnValue);

private:
	std::vector<uint8_t> maskBinary;
	flexbuffers::Map maskMap = flexbuffers::Reference().AsMap();
};

}
