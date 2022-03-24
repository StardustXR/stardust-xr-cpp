#include "clientmanager.hpp"
#include "../nodetypes/drawable/drawable.hpp"
#include "scenegraphpropagation.hpp"
#include <memory>
#include <mutex>
#include <vector>

namespace StardustXRServer {

ClientManager::ClientManager() : StardustXR::MessengerManager() {}

void ClientManager::clientConnected(int fd) {
	const std::lock_guard<std::mutex> lock(connectedClientsMutex);
	newlyConnectedClients.emplace_back(fd);
}

void ClientManager::handleNewlyConnectedClients() {
	const std::lock_guard<std::mutex> lock(connectedClientsMutex);
	
	for(int newlyConnectedClient : newlyConnectedClients) {
		Client *client = new Client(*this, newlyConnectedClient);
		clients.emplace_back(client);
		client->startMessenger();
	}
	newlyConnectedClients.clear();
}

void ClientManager::handleDisconnectedClients() {
	const std::lock_guard<std::mutex> lock(connectedClientsMutex);
	clients.erase(std::remove_if(clients.begin(), clients.end(), [](std::unique_ptr<Client> &client) {
		bool disconnected = !!client->disconnected; // !! inverts the atomic bool twice, gently converting it to a regular bool
		return disconnected;
	}), clients.end());
}

void ClientManager::callClientsDebug() {
	for(auto &client : clients) {
		client->scenegraphPropagate("", ScenegraphDebugFunction);
	}
}

}
