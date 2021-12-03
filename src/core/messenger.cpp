#include "messenger.hpp"
#include "client.hpp"
#include "clientmanager.hpp"
#include <unistd.h>

namespace StardustXRServer {

Messenger::Messenger(Client *client, int fd) : StardustXR::Messenger(fd, &client->scenegraph) {
	this->client = client;
}
Messenger::~Messenger() {
	close(fd);
}

void Messenger::onPipeBreak() {
	client->disconnected = true;
	pipeBroke = true;
}

}
