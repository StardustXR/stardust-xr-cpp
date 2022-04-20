#pragma once

#include <sys/epoll.h>
#include <map>
#include <mutex>
#include <thread>
#include <vector>

#include <stardustxr/server/socket.hpp>
#include "client.hpp"

namespace StardustXRServer {

class EventLoop {
public:
	EventLoop(bool &successful);
	~EventLoop();

	const int epollFD;

	void addConnection(Connection *connection);
	void callClientsDebug();
	void stop();

private:
	bool successful;

	std::vector<std::unique_ptr<Connection>> connections;
	void closeConnection(Connection *connection);
	Connection *internalConnection;

	StardustXRServer::SocketHandler socketHandler;
	std::thread eventThread;
	void eventLoop();
};

}
