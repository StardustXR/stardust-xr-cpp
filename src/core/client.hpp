#pragma once

#include <atomic>

#include "messenger.hpp"
#include "scenegraph.hpp"
#include "scenegraphpropagation.hpp"

namespace StardustXRServer {

class ClientManager;

class Client {
public:
	explicit Client(int fd, ClientManager *messengerManager);
	~Client();

	void startHandler();
	void scenegraphPropagate(std::string name, PropagateFunction &function);

	pid_t pid;

	uint32_t clientNumber;
	std::atomic<bool> disconnected{false};
	Messenger messenger;
	Scenegraph scenegraph;
	ClientManager *manager;

	static uint32_t clientCount;
};

}
