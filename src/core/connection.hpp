#pragma once

#include <sys/epoll.h>

namespace StardustXRServer {

class EventLoop;

class Connection {
public:
	Connection(EventLoop *eventLoop, int fd, uint32_t dispatchEvents);
	void startPoll();
	virtual ~Connection();
	virtual bool dispatch();
	const int fd;
	EventLoop *const eventLoop;
private:
	bool polling = false;
	epoll_event event;
};

}
