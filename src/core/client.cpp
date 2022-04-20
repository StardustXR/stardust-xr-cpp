#include "client.hpp"
#include "eventloop.hpp"
#include "scenegraph.hpp"
#include "scenegraphpropagation.hpp"

namespace StardustXRServer {

Client::Client(int epollFD, int fd) :
Connection(epollFD, fd, EPOLLIN),
fd(fd),
scenegraph(this),
messenger(fd, &scenegraph) {}

Client::~Client() {
	::close(fd);
	printf("Disconnecting client %p\n", this);
}

bool Client::dispatch() {
	return messenger.dispatch();
}

void Client::scenegraphPropagate(std::string name, PropagateFunction &function) {
	scenegraph.root.propagate(name, function);
}

pid_t Client::getPID() {
	if(fd > 0) {
		struct ucred ucred;
		uint len = sizeof(struct ucred);
		getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &ucred, &len);

		return ucred.pid;
	} else {
		return 0;
	}
}

}
