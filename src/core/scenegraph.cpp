#include "client.hpp"

#include "scenegraph.hpp"
#include "scenegraphpropagation.hpp"

namespace StardustXRServer {

Scenegraph::Scenegraph(Client *client) : root(client) {}

void Scenegraph::onPathStep(std::string path, std::function<void(std::string &, bool &)> pathStepFunction) {
	std::istringstream stream(path);
	stream.get(); //Remove beginning slash

	bool stop = false;
	std::string pathStep;
	while(std::getline(stream, pathStep, '/')) {
		pathStepFunction(pathStep, stop);
		if(stop)
			break;
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

std::vector<uint8_t> Scenegraph::executeMethod(std::string path, std::string method, flexbuffers::Reference args, bool returnValue) {
	//Find the node referenced by the path string
	Node *currentNode = &root;
	this->onPathStep(path, [&](std::string &pathStep, bool &stop) {
		stop = currentNode == nullptr;
		if(stop)
			return;
		currentNode = currentNode->children[pathStep].get();
		stop = currentNode == nullptr;
	});

	if(currentNode == nullptr) {
		printf("Node %s not found\n", path.c_str());
		return std::vector<uint8_t>();
	}

	if(currentNode->methods[method] == nullptr) {
		printf("Method %s on node %s not found\n", method.c_str(), path.c_str());
		return std::vector<uint8_t>();
	}
	return (currentNode->methods[method])(root.client, args, returnValue);
}

void Scenegraph::addNode(std::string path, Node *node) {
	//Get the name of the node to create
	std::string lastNodeName = path.substr(path.find_last_of("/")+1);
	Node *currentNode = &root;

	this->onPathStep(path, [&](std::string &pathStep, bool &stop) {
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

	this->onPathStep(path, [&](std::string &pathStep, bool &stop) {
		if(!doesNotExist)
			currentNode = currentNode->children[pathStep].get();

		if(!doesNotExist && currentNode == nullptr)
			doesNotExist = true;
		return true;
	});

	return currentNode;
}

} // namespace StardustXRServer
