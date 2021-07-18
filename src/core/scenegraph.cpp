#include "../globals.h"

#include "scenegraph.hpp"
#include "scenegraphpropagation.hpp"

namespace StardustXRServer {

Scenegraph::Scenegraph(Client *client) : root(client) {}
Scenegraph::~Scenegraph() {}

void Scenegraph::onPathStep(std::string path, std::function<void(std::string)> pathStepFunction) {
	std::istringstream stream(path);
	stream.get(); //Remove beginning slash

	std::string pathStep;
	while(std::getline(stream, pathStep, '/')) {
		pathStepFunction(pathStep);
	}
}

void Scenegraph::sendSignal(std::string path, std::string method, flexbuffers::Reference data) {
	this->executeMethod(path, method, data, false);
}
std::vector<uint8_t> Scenegraph::executeMethod(std::string path, std::string method, flexbuffers::Reference args) {
	return this->executeMethod(path, method, args, true);
}

void Scenegraph::executeRemoteMethod(std::string remotePath, std::string remoteMethod, std::vector<uint8_t> args, void *extraData, ServerCallback callback) {
	root.client->messenger.executeRemoteMethod(remotePath.c_str(), remoteMethod.c_str(), args, [&](flexbuffers::Reference data) {
		callback(data, extraData);
	});
}

void Scenegraph::handleClientDisconnect(Client *client) {
	PropagateFunction messengerDeletionFunction = [&](std::string name, Node *node) {
		node->handleClientDisconnect(client);
		return true;
	};
	root.propagate("", messengerDeletionFunction);
}

std::vector<uint8_t> Scenegraph::executeMethod(std::string path, std::string method, flexbuffers::Reference args, bool returnValue) {
	//Find the node referenced by the path string
	Node *currentNode = &root;
	this->onPathStep(path, [&](std::string pathStep) {
		currentNode = currentNode->children[pathStep].get();
		if(currentNode == nullptr) {
			printf("Node %s not found\n", pathStep.c_str());
			return;
		}
	});

	while(currentNode == nullptr) {}

	if(currentNode->methods[method] == nullptr) {
		printf("Method %s on node %s not found\n", method.c_str(), path.c_str());
		return std::vector<uint8_t>();
	}
	return (currentNode->methods[method])(args, returnValue);
}

void Scenegraph::addNode(std::string path, Node *node) {
	//Get the name of the node to create
	std::string lastNodeName = path.substr(path.find_last_of("/")+1);
	Node *currentNode = &root;

	this->onPathStep(path, [&](std::string pathStep) {

		if(pathStep == lastNodeName)
			currentNode->children.emplace(pathStep, node);
		else if(currentNode->children.find(pathStep) == currentNode->children.end())
			currentNode->children.emplace(pathStep, new Node(root.client));

		currentNode->children[pathStep]->parent = currentNode;
		currentNode = currentNode->children[pathStep].get();

	});
}

Node *Scenegraph::findNode(std::string path) {
	std::string lastNodeName = path.substr(path.find_last_of("/")+1);
	Node *currentNode = &root;
	bool doesNotExist = false;

	this->onPathStep(path, [&](std::string pathStep) {
		if(!doesNotExist)
			currentNode = currentNode->children[pathStep].get();

		if(!doesNotExist && currentNode == nullptr)
			doesNotExist = true;
	});

	return currentNode;
}

} // namespace StardustXRServer
