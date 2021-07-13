#include "clientmanager.hpp"
#include "../nodetypes/graphical/drawablenode.hpp"
#include "scenegraphpropagation.hpp"
#include <vector>

namespace StardustXRServer {

ClientManager::ClientManager(const char *socketPath) : StardustXR::MessengerManager(socketPath) {}
ClientManager::~ClientManager() {}

void ClientManager::clientConnected(int inFD, int outFD) {
	Client *client = new Client(inFD, outFD, this);
	clients.push_back(client);
	client->startHandler();
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
	disconnectedClients.push_back(client);
}
void ClientManager::handleDisconnectedClients() {
	for(Client *disconnectedClient : disconnectedClients) {
		disconnectedClient->scenegraph.handleClientDisconnect(disconnectedClient);
		for(auto clientsIterator = clients.begin(); clientsIterator != clients.end(); ++clientsIterator) {
			if(*clientsIterator == disconnectedClient) {
				clients.erase(clientsIterator);
			}
			break;
		}
		delete disconnectedClient;
	}
	if(disconnectedClients.size() > 0)
		disconnectedClients = std::vector<Client *>();
}

}
