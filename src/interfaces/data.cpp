#include "data.hpp"

#include "../core/client.hpp"
#include "../nodetypes/spatial/spatial.hpp"
#include "../nodetypes/fields/field.hpp"
#include "../nodetypes/data/sender.hpp"
#include "../nodetypes/data/receiver.hpp"
#include "../util/flex.hpp"

using namespace sk;

namespace StardustXRServer {

DataInterface::DataInterface(Client *client) : Node(client, false) {
	addChild("sender", new Node(client));
	addChild("receiver", new Node(client));

	STARDUSTXR_NODE_METHOD("createNonSpatialSender", &DataInterface::createNonSpatialSender)
	STARDUSTXR_NODE_METHOD("createNonSpatialReceiver", &DataInterface::createNonSpatialReceiver)
}

std::vector<uint8_t> DataInterface::createNonSpatialSender(Client *, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec = data.AsVector();

	std::string name            = flexVec[0].AsString().str();
	Spatial *space              = this->client->scenegraph.findNode<Spatial>(flexVec[1].AsString().str());
	sk::pose_t transform        = FlexToSKPose(flexVec[2].AsTypedVector(), flexVec[3].AsTypedVector());

	NonSpatialSender *sender = new NonSpatialSender(client, space, transform);
	children["sender"]->addChild(name, sender);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> DataInterface::createNonSpatialReceiver(Client *, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec = data.AsVector();

	std::string name            = flexVec[0].AsString().str();
	Field *field                = this->client->scenegraph.findNode<Field>(flexVec[1].AsString().str());
	Spatial *spatialParent      = this->client->scenegraph.findNode<Spatial>(flexVec[2].AsString().str());
	sk::pose_t transform        = FlexToSKPose(flexVec[3].AsTypedVector(), flexVec[4].AsTypedVector());
	std::string callbackPath    = flexVec[5].AsString().str();
	std::string callbackMethod  = flexVec[6].AsString().str();

	NonSpatialReceiver *receiver = new NonSpatialReceiver(client, spatialParent, transform, field, callbackPath, callbackMethod);
	children["receiver"]->addChild(name, receiver);

	return std::vector<uint8_t>();
}

}
