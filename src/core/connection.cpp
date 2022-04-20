#include "connection.hpp"

namespace StardustXRServer {

Connection::Connection(int epollFD, int fd, uint32_t dispatchEvents) :
fd(fd),
epollFD(epollFD),
event(epoll_event {
	.events = dispatchEvents,
	.data = {this},
}) {}
void Connection::startPoll() {
	epoll_ctl(epollFD, EPOLL_CTL_ADD, fd, &event);
}
Connection::~Connection() {
	epoll_ctl(epollFD, EPOLL_CTL_DEL, fd, &event);
}

bool Connection::dispatch(){
	return true;
}

}
