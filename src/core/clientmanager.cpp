#include "clientmanager.hpp"
#include "../nodetypes/graphical/drawablenode.hpp"
#include "scenegraphpropagation.hpp"
#include <vector>

namespace StardustXRServer {

ClientManager::ClientManager(const char *socketPath) : StardustXR::MessengerManager(socketPath) {}
ClientManager::~ClientManager() {}

void ClientManager::clientConnected(int inFD, int outFD) {
	const std::lock_guard<std::mutex> lock(connectedClientsMutex);
	newlyConnectedClients.emplace_back(inFD, outFD);
}

void ClientManager::callClientsUpdate() {
	for(Client *client : clients) {
		client->scenegraphPropagate("", ScenegraphUpdateFunction);
	}
}

void ClientManager::callClientsDraw() {
	for(Client *client : clients) {
		client->scenegraphPropagate("", ScenegraphDrawFunction);
	}
}

void ClientManager::disconnectClient(Client *client) {
	const std::lock_guard<std::mutex> lock(connectedClientsMutex);

	disconnectedClients.push_back(client);
}
void ClientManager::handleDisconnectedClients() {
	const std::lock_guard<std::mutex> lock(connectedClientsMutex);

	if(disconnectedClients.size() == 0)
		return;
	
	for(auto &disconnectedClient : disconnectedClients) {
		disconnectedClient->scenegraph.handleClientDisconnect(disconnectedClient);
		for(auto clientsIterator = clients.begin(); clientsIterator != clients.end(); ++clientsIterator) {
			if(*clientsIterator == disconnectedClient) {
				clients.erase(clientsIterator);
			}
			break;
		}
		delete disconnectedClient;
	}
	disconnectedClients.clear();
}

void ClientManager::callClientsUpdate() {
	for(Client *client : clients) {
		client->scenegraphPropagate("", ScenegraphUpdateFunction);
	}
}

void ClientManager::callClientsDraw() {
	for(Client *client : clients) {
		client->scenegraphPropagate("", ScenegraphDrawFunction);
	}
}

}
