#include "client.hpp"
#include "clientmanager.hpp"
#include "messenger.hpp"
#include "scenegraph.hpp"
#include "scenegraphpropagation.hpp"

namespace StardustXRServer {

uint32_t Client::clientCount = 0;

Client::Client(int inFD, int outFD, ClientManager *clientManager) :
messenger(this, inFD, outFD),
scenegraph(this, clientManager->pidCache.find(messenger.clientPID) == clientManager->pidCache.end() ? matrix_identity : clientManager->pidCache[messenger.clientPID]) {
	this->manager = clientManager;
	if(messenger.clientPID != 0)
		clientManager->pidCache.erase(messenger.clientPID);

	clientNumber = clientCount;
	clientCount++;
}
Client::~Client() {
	printf("Disconnecting client %p\n", this);
}

void Client::scenegraphPropagate(std::string name, PropagateFunction &function) {
	scenegraph.root.propagate(name, function);
}

void Client::startHandler() {
	messenger.startHandler();
}

}
