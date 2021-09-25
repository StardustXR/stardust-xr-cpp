#pragma once

#include <stardustxr/common/scenegraph.hpp>
#include "../nodetypes/node.hpp"

namespace StardustXRServer {

typedef std::function<void(flexbuffers::Reference, void *)> ServerCallback;

class Client;

class Scenegraph : public StardustXR::Scenegraph {
public:
	explicit Scenegraph(Client *client);
	~Scenegraph();

	void sendSignal(std::string path, std::string method, flexbuffers::Reference data);
	std::vector<uint8_t> executeMethod(std::string path, std::string method, flexbuffers::Reference args);

	void executeRemoteMethod(std::string remotePath, std::string remoteMethod, std::vector<uint8_t> args, void *extraData, ServerCallback callback);

	void addNode(std::string path, Node *node);
	Node *findNode(std::string path);
	template<class T>
	T *findNode(std::string path) {
		return dynamic_cast<T *>(findNode(path));
	}

	Node root;
protected:
	std::vector<uint8_t> executeMethod(std::string path, std::string method, flexbuffers::Reference args, bool returnValue);
	void onPathStep(std::string path, std::function<void(std::string &, bool &)> pathStepFunction);
};

} // namespace StardustXR
