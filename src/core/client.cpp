#include "client.hpp"
#include "clientmanager.hpp"
#include "messenger.hpp"
#include "scenegraph.hpp"
#include "scenegraphpropagation.hpp"

namespace StardustXRServer {

Client::Client(int inFD, int outFD, ClientManager *clientManager) :
scenegraph(this),
messenger(this, inFD, outFD) {
	this->manager = clientManager;
}
Client::~Client() {}

void Client::scenegraphPropagate(std::string name, PropagateFunction &function) {
	scenegraph.root.propagate(name, function);
}

void Client::startHandler() {
	messenger.startHandler();
}

}
