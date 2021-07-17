#pragma once

#include <stardustxr/server/messengermanager.hpp>
#include <vector>

#include "client.hpp"
#include <mutex>

namespace StardustXRServer {

class ClientManager : public StardustXR::MessengerManager {
public:
	explicit ClientManager(const char *socketPath = "/tmp/stardust.sock");
	~ClientManager();

	std::vector<Client *> clients;
	void handleNewlyConnectedClients();
	void disconnectClient(Client *client);
	void handleDisconnectedClients();

	void callClientsUpdate();
	void callClientsDraw();

protected:
	std::mutex connectedClientsMutex;

	std::vector<std::pair<int, int>> newlyConnectedClients;
	std::vector<Client *> disconnectedClients;

	void clientConnected(int inFD, int outFD);
};

}