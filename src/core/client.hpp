#pragma once

#include <atomic>

#include "messenger.hpp"
#include "scenegraph.hpp"
#include "scenegraphpropagation.hpp"

namespace StardustXRServer {

class ClientManager;

class Client {
public:
	explicit Client(int inFD, int outFD, ClientManager *messengerManager);
	~Client();

	void startHandler();
	void scenegraphPropagate(std::string name, PropagateFunction &function);

	uint32_t clientNumber;
	std::atomic<bool> disconnected{false};
	Messenger messenger;
	Scenegraph scenegraph;
	ClientManager *manager;

	static uint32_t clientCount;
};

}
