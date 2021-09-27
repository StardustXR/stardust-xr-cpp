#include "root.hpp"
#include "../core/client.hpp"

#include "drawable.hpp"
#include "environment.hpp"
#include "field.hpp"
#include "input.hpp"
#include "lifecycle.hpp"
#include "spatial.hpp"

using namespace sk;

namespace StardustXRServer {

RootInterface::RootInterface(Client *client) : Node(client, false) {
	STARDUSTXR_NODE_METHOD("disconnect", &RootInterface::disconnect)

	this->addChild("drawable",    new DrawableInterface(client));
	this->addChild("environment", new EnvironmentInterface(client));
	this->addChild("field",       new FieldInterface(client));
	this->addChild("input",       new InputInterface(client));
	this->addChild("lifecycle",   new LifeCycleInterface(client));
	this->addChild("spatial",     new SpatialFactory(client));
}

std::vector<uint8_t> RootInterface::disconnect(flexbuffers::Reference data, bool) {
	this->client->disconnected = true;
	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
