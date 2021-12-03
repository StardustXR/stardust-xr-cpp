#include "client.hpp"
#include "clientmanager.hpp"
#include "messenger.hpp"
#include "scenegraph.hpp"
#include "scenegraphpropagation.hpp"

namespace StardustXRServer {

uint32_t Client::clientCount = 0;

Client::Client(int fd, ClientManager *clientManager) :
messenger(this, fd),
scenegraph(this) {
	this->manager = clientManager;

	if(fd > 0) {
		struct ucred ucred;
		uint len = sizeof(struct ucred);
		getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &ucred, &len);

		pid = ucred.pid;

		if(clientManager->pidCache.find(pid) != clientManager->pidCache.end())
			scenegraph.root.setTransformMatrix(clientManager->pidCache[pid]);

		clientManager->pidCache.erase(pid);
	}

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
