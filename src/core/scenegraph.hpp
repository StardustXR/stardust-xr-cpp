#pragma once

#include <stardustxr/server/scenegraph.hpp>
#include "../nodetypes/node.hpp"

namespace StardustXRServer {

typedef std::function<void(flexbuffers::Reference, void *)> ServerCallback;

class Client;

class Scenegraph : public StardustXR::ServerScenegraph {
public:
	explicit Scenegraph(Client *client);
	~Scenegraph();

	void sendSignal(std::string path, std::string method, flexbuffers::Reference data);
	std::vector<uint8_t> executeMethod(std::string path, std::string method, flexbuffers::Reference args);

	void executeRemoteMethod(std::string remotePath, std::string remoteMethod, std::vector<uint8_t> args, void *extraData, ServerCallback callback);

	void handleClientDisconnect(Client *client);

	void addNode(std::string path, Node *node);
	Node *findNode(std::string path);

	Node root;
protected:
	std::vector<uint8_t> executeMethod(std::string path, std::string method, flexbuffers::Reference args, bool returnValue);
	void onPathStep(std::string path, std::function<void(std::string)> pathStepFunction);
};

} // namespace StardustXR
