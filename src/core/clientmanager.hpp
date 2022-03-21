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

	static std::vector<std::unique_ptr<Client>> clients;
	static void handleNewlyConnectedClients();
	static void handleDisconnectedClients();

	static void callClientsDebug();

	static std::mutex pidCacheMutex;
	static std::map<pid_t, matrix> pidCache;

protected:
	static std::mutex connectedClientsMutex;
	static std::vector<int> newlyConnectedClients;

	void clientConnected(int fd);
	static void clientDisconnected(int fd);
};

}
