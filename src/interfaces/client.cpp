#include "client.hpp"
#include "../core/client.hpp"

using namespace sk;

namespace StardustXRServer {

ClientInterface::ClientInterface(Client *client) : Node(client, false) {
	STARDUSTXR_NODE_METHOD("disconnect", &ClientInterface::disconnect)
}

std::vector<uint8_t> ClientInterface::disconnect(flexbuffers::Reference data, bool) {
	this->client->disconnected = true;
	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
