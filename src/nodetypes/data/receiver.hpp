#pragma once

#include "../core/noderef.hpp"
#include "../spatial/spatial.hpp"
#include "../fields/field.hpp"

namespace StardustXRServer {

class NonSpatialSender;

class NonSpatialReceiver : public Spatial {
public:
	NonSpatialReceiver(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, Field *field, std::string callbackPath, std::string callbackMethod);
	~NonSpatialReceiver();

	NodeRef field;
	std::string callbackPath;
	std::string callbackMethod;

	static std::mutex receiversMutex;
	static std::vector<NonSpatialReceiver *> receivers;
	static std::vector<std::string> makeAliases(Node *parent);

	void sendData(NonSpatialSender *sender, const uint8_t *data, size_t dataSize);
};

}
