#pragma once

#include <stardustxr/server/messengermanager.hpp>
#include <vector>

#include "client.hpp"

namespace StardustXRServer {

class ClientManager : public StardustXR::MessengerManager {
public:
	explicit ClientManager(const char *socketPath = "/tmp/stardust.sock");
	~ClientManager();

	std::vector<Client *> clients;
	void callClientsUpdate();
	void callClientsDraw();

	void disconnectClient(Client *client);
	void handleDisconnectedClients();

protected:
	std::vector<Client *> disconnectedClients;

	void clientConnected(int inFD, int outFD);
};

}