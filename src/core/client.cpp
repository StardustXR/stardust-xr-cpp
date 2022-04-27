#include "client.hpp"
#include "eventloop.hpp"
#include "scenegraph.hpp"
#include "scenegraphpropagation.hpp"

namespace StardustXRServer {

Client::Client(EventLoop *eventLoop, int fd) :
Connection(eventLoop, fd, EPOLLIN),
fd(fd),
scenegraph(this),
messenger(fd, &scenegraph) {}

Client::~Client() {
	::close(fd);
	printf("Disconnected client %p\n", this);
	fflush(stdout);
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
