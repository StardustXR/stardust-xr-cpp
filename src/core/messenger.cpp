#include "messenger.hpp"
#include "client.hpp"
#include "clientmanager.hpp"

namespace StardustXRServer {

Messenger::Messenger(Client *client, int readFD, int writeFD) : StardustXR::Messenger(readFD, writeFD, &client->scenegraph) {
	this->client = client;
}
Messenger::~Messenger() {}

void Messenger::onPipeBreak() {
	client->manager->disconnectClient(client);
}

}