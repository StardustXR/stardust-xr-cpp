#pragma once

#include <mutex>
#include <memory>
#include <vector>

#include <stardustxr/server/messengermanager.hpp>
#include "client.hpp"

namespace StardustXRServer {

class ClientManager : public StardustXR::MessengerManager {
public:
	explicit ClientManager(const char *socketPath = "/tmp/stardust.sock");
	~ClientManager();

	std::vector<std::unique_ptr<Client>> clients;
	void handleNewlyConnectedClients();
	void handleDisconnectedClients();

	void callClientsUpdate();
	void callClientsDebug();
	void callClientsDraw();

protected:
	std::mutex connectedClientsMutex;

	std::vector<std::pair<int, int>> newlyConnectedClients;

	void clientConnected(int inFD, int outFD);
};

}