#pragma once

#include <stardustxr/server/messenger.hpp>
#include "scenegraph.hpp"
#include "scenegraphpropagation.hpp"

namespace StardustXRServer {

class ClientManager;

class Client {
public:
	explicit Client(uint clientID, int inFD, int outFD, StardustXRServer::ClientManager *messengerManager);
	~Client();

	void startHandler();
	void scenegraphPropagate(std::string name, PropagateFunction &function);
	
	StardustXR::ServerMessenger messenger;
	Scenegraph scenegraph;
};

}