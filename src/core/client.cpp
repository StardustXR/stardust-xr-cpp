#include "client.hpp"
#include "clientmanager.hpp"
#include "messenger.hpp"
#include "scenegraph.hpp"
#include "scenegraphpropagation.hpp"

namespace StardustXRServer {

uint32_t Client::clientCount = 0;

Client::Client(int fd) :
fd(fd),
pid(getPID(fd)),
messenger(this, fd),
scenegraph(this) {

	if(pid > 0) {
		if(ClientManager::pidCache.find(pid) != ClientManager::pidCache.end())
			scenegraph.root.setTransformMatrix(ClientManager::pidCache[pid]);

		ClientManager::pidCache.erase(pid);
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

pid_t Client::getPID(int fd) {
	if(fd > 0) {
		struct ucred ucred;
		uint len = sizeof(struct ucred);
		getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &ucred, &len);

		return ucred.pid;
	} else {
		return 0;
	}
}

void Client::startMessenger() {
	messenger.startHandler();
}

}
