#pragma once

#include <atomic>

#include "messenger.hpp"
#include "scenegraph.hpp"
#include "scenegraphpropagation.hpp"

namespace StardustXRServer {

class ClientManager;

class Client {
public:
	explicit Client(int fd);
	~Client();

	void startMessenger();
	void scenegraphPropagate(std::string name, PropagateFunction &function);

	const int fd;
	const pid_t pid;

	static pid_t getPID(int fd);

	uint32_t clientNumber;
	std::atomic<bool> disconnected{false};
	Messenger messenger;
	Scenegraph scenegraph;

	static uint32_t clientCount;
};

}
