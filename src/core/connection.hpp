#pragma once

#include <sys/epoll.h>

namespace StardustXRServer {

class Connection {
public:
	Connection(int epollFD, int fd, uint32_t dispatchEvents);
	void startPoll();
	virtual ~Connection();
	virtual bool dispatch();
	const int fd;
private:
	const int epollFD;
	epoll_event event;
};

}
