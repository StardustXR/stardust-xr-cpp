#include "data.hpp"

#include "../nodetypes/spatial/spatial.hpp"
#include "../nodetypes/fields/field.hpp"
#include "../nodetypes/data/sender.hpp"
#include "../nodetypes/data/receiver.hpp"

#include "../core/client.hpp"

using namespace sk;

namespace StardustXRServer {

DataInterface::DataInterface(Client *client) : Node(client, false) {
	addChild("sender", new Node(client));
	addChild("receiver", new Node(client));

	STARDUSTXR_NODE_METHOD("createNonSpatialSender", &DataInterface::createNonSpatialSender)
	STARDUSTXR_NODE_METHOD("createNonSpatialReceiver", &DataInterface::createNonSpatialReceiver)
}

std::vector<uint8_t> DataInterface::createNonSpatialSender(Client *, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec      = data.AsVector();
	std::string name                 = flexVec[0].AsString().str();
	Spatial *space                   = this->client->scenegraph.findNode<Spatial>(flexVec[1].AsString().str());
	flexbuffers::TypedVector flexPos = flexVec[2].AsTypedVector();
	flexbuffers::TypedVector flexRot = flexVec[3].AsTypedVector();

	sk::vec3 pos = {
		flexPos[0].AsFloat(),
		flexPos[1].AsFloat(),
		flexPos[2].AsFloat()
	};
	sk::quat rot = {
		flexRot[0].AsFloat(),
		flexRot[1].AsFloat(),
		flexRot[2].AsFloat(),
		flexRot[3].AsFloat()
	};

	NonSpatialSender *sender = new NonSpatialSender(client, space, pose_t{pos, rot});
	children["sender"]->addChild(name, sender);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> DataInterface::createNonSpatialReceiver(Client *, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector vector            = data.AsVector();
	std::string name                      = vector[0].AsString().str();
	Field *field                          = this->client->scenegraph.findNode<Field>(vector[1].AsString().str());
	Spatial *spatialParent                = this->client->scenegraph.findNode<Spatial>(vector[2].AsString().str());
	flexbuffers::TypedVector flexPosition = vector[3].AsTypedVector();
	flexbuffers::TypedVector flexRotation = vector[4].AsTypedVector();
	std::string callbackPath              = vector[5].AsString().str();
	std::string callbackMethod            = vector[6].AsString().str();

	vec3 position = {
		flexPosition[0].AsFloat(),
		flexPosition[1].AsFloat(),
		flexPosition[2].AsFloat()
	};
	quat rotation = {
		flexRotation[0].AsFloat(),
		flexRotation[1].AsFloat(),
		flexRotation[2].AsFloat(),
		flexRotation[3].AsFloat()
	};

	NonSpatialReceiver *receiver = new NonSpatialReceiver(client, spatialParent, pose_t{position, rotation}, field, callbackPath, callbackMethod);
	children["receiver"]->addChild(name, receiver);

	return std::vector<uint8_t>();
}

}
