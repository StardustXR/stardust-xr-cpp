#ifndef STARDUSTXR_SCENEGRAPH_HPP
#define STARDUSTXR_SCENEGRAPH_HPP

#include <stardustxr/server/scenegraph.hpp>
#include "../nodetypes/node.hpp"

namespace StardustXRServer {

class Scenegraph : public StardustXR::ServerScenegraph {
public:
	Scenegraph() : StardustXR::ServerScenegraph() {}

	void sendSignal(int sessionID, std::string path, std::string method, flexbuffers::Reference data);
	std::vector<uint8_t> executeMethod(int sessionID, std::string path, std::string method, flexbuffers::Reference args);

	void addNode(std::string path, Node *node);
	Node root;
protected:
	std::vector<uint8_t> executeMethod(int sessionID, std::string path, std::string method, flexbuffers::Reference args, bool returnValue);
	void onPathStep(std::string path, std::function<void(std::string)> pathStepFunction);
};

} // namespace StardustXR

#endif //STARDUSTXR_SCENEGRAPH_HPP
