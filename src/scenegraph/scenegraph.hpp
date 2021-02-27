#pragma once

#include <stardustxr/server/scenegraph.hpp>
#include "../nodetypes/node.hpp"

namespace StardustXRServer {

typedef std::function<void(uint, flexbuffers::Reference, void *)> ServerCallback;

class Scenegraph : public StardustXR::ServerScenegraph {
public:
	Scenegraph() : StardustXR::ServerScenegraph() {}

	void sendSignal(int sessionID, std::string path, std::string method, flexbuffers::Reference data);
	std::vector<uint8_t> executeMethod(int sessionID, std::string path, std::string method, flexbuffers::Reference args);

	void executeRemoteMethod(uint sessionID, std::string remotePath, std::string remoteMethod, std::vector<uint8_t> args, void *extraData, ServerCallback callback);

	void handleMessengerDeletion(uint sessionID);

	void addNode(std::string path, Node *node);
	Node *findNode(std::string path);

	Node root;
protected:
	std::vector<uint8_t> executeMethod(int sessionID, std::string path, std::string method, flexbuffers::Reference args, bool returnValue);
	void onPathStep(std::string path, std::function<void(std::string)> pathStepFunction);
};

} // namespace StardustXR
