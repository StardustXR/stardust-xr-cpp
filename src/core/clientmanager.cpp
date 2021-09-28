#include "clientmanager.hpp"
#include "../nodetypes/drawable/drawablenode.hpp"
#include "scenegraphpropagation.hpp"
#include <memory>
#include <mutex>
#include <vector>

namespace StardustXRServer {

ClientManager::ClientManager(const char *socketPath) : StardustXR::MessengerManager(socketPath) {}

void ClientManager::clientConnected(int inFD, int outFD) {
	const std::lock_guard<std::mutex> lock(connectedClientsMutex);
	newlyConnectedClients.emplace_back(inFD, outFD);
}

void ClientManager::handleNewlyConnectedClients() {
	const std::lock_guard<std::mutex> lock(connectedClientsMutex);
	
	if(newlyConnectedClients.size() == 0)
		return;
	
	for(auto newlyConnectedClient : newlyConnectedClients) {
		Client *client = new Client(newlyConnectedClient.first, newlyConnectedClient.second, this);
		clients.emplace_back(client);
		client->startHandler();
	}
	newlyConnectedClients.clear();
}

void ClientManager::handleDisconnectedClients() {
	if(clients.size() == 0)
		return;

	clients.erase(std::remove_if(clients.begin(), clients.end(), [](std::unique_ptr<Client> &client) {
		bool disconnected = !!client->disconnected; // !! inverts the atomic bool twice, gently converting it to a regular bool
		return disconnected;
	}), clients.end());
}

void ClientManager::callClientsUpdate() {
	for(auto &client : clients) {
		client->scenegraphPropagate("", ScenegraphUpdateFunction);
	}
}

void ClientManager::callClientsDebug() {
	for(auto &client : clients) {
		client->scenegraphPropagate("", ScenegraphDebugFunction);
	}
}

void ClientManager::callClientsDraw() {
	for(auto &client : clients) {
		client->scenegraphPropagate("", ScenegraphDrawFunction);
	}
}

}
