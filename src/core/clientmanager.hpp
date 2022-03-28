#pragma once

#include <map>
#include <mutex>
#include <vector>

#include <stardustxr/server/messengermanager.hpp>
#include "client.hpp"

namespace StardustXRServer {

class ClientManager : public StardustXR::MessengerManager {
public:
	explicit ClientManager();

	std::vector<std::unique_ptr<Client>> clients;
	void handleNewlyConnectedClients();
	void handleDisconnectedClients();

	void callClientsDebug();

	std::mutex pidCacheMutex;
	std::map<pid_t, sk::matrix> pidCache;

protected:
	std::mutex connectedClientsMutex;
	std::vector<int> newlyConnectedClients;

	void clientConnected(int fd);
};

}
