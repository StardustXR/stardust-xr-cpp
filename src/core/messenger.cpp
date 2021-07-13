#include "messenger.hpp"
#include "client.hpp"
#include "clientmanager.hpp"

namespace StardustXRServer {

Messenger::Messenger(Client *client, int readFD, int writeFD) : StardustXR::ServerMessenger(readFD, writeFD, &client->scenegraph, client->manager) {
	this->client = client;
}
Messenger::~Messenger() {}

void Messenger::onPipeBreak() {
	client->manager->disconnectClient(client);
}

}