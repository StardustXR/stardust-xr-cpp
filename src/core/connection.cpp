#include "connection.hpp"
#include "eventloop.hpp"

namespace StardustXRServer {

Connection::Connection(EventLoop *eventLoop, int fd, uint32_t dispatchEvents) :
fd(fd),
eventLoop(eventLoop),
event(epoll_event {
	.events = dispatchEvents,
	.data = {this},
}) {}
void Connection::startPoll() {
	epoll_ctl(eventLoop->epollFD, EPOLL_CTL_ADD, fd, &event);
	polling = true;
}
Connection::~Connection() {
	if(polling)
		epoll_ctl(eventLoop->epollFD, EPOLL_CTL_DEL, fd, &event);
}

bool Connection::dispatch(){
	return true;
}

}
