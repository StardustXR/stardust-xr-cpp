#pragma once

#include "../core/typednoderef.hpp"
#include "../core/callback.hpp"
#include "../spatial/spatial.hpp"
#include "../fields/field.hpp"

namespace StardustXRServer {

class PulseSender;

class PulseReceiver : public Spatial {
public:
	PulseReceiver(Client *client, Spatial *spatialParent, sk::pose_t pose, Field *field, std::string callbackPath, std::string callbackMethod);
	~PulseReceiver();

	TypedNodeRef<Field> field;
	Callback eventCallback;

	static Registry<PulseReceiver> receivers;
	static std::vector<std::string> makeAliases(Node *parent);

	std::vector<uint8_t> maskBinary;
	void sendData(PulseSender *sender, const std::vector<uint8_t> &data);

	std::vector<uint8_t> getMask(Client *callingClient, flexbuffers::Reference data, bool returnValue);
	std::vector<uint8_t> setMask(Client *callingClient, flexbuffers::Reference data, bool returnValue);

private:
	flexbuffers::Map maskMap = flexbuffers::Reference().AsMap();
};

}
