#include "messenger.hpp"
#include "client.hpp"
#include "clientmanager.hpp"
#include <unistd.h>

namespace StardustXRServer {

Messenger::Messenger(Client *client, int readFD, int writeFD) : StardustXR::Messenger(readFD, writeFD, &client->scenegraph) {
	this->client = client;
	if(readFD != 0)
		read(readFD, &clientPID, sizeof(pid_t));
}
Messenger::~Messenger() {
	close(messageReadFD);
	close(messageWriteFD);
}

void Messenger::onPipeBreak() {
	client->disconnected = true;
	pipeBroke = true;
}

}
