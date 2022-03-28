#include "field.hpp"
#include "../core/client.hpp"
#include "../nodetypes/fields/boxfield.hpp"
#include "../nodetypes/fields/cylinderfield.hpp"
#include "../nodetypes/fields/spherefield.hpp"
#include "../util/flex.hpp"

using namespace sk;

namespace StardustXRServer {

FieldInterface::FieldInterface(Client *client) : Node(client, false) {
	STARDUSTXR_NODE_METHOD("createBoxField", &FieldInterface::createBoxField)
	STARDUSTXR_NODE_METHOD("createCylinderField", &FieldInterface::createCylinderField)
	STARDUSTXR_NODE_METHOD("createSphereField", &FieldInterface::createSphereField)
}

std::vector<uint8_t> FieldInterface::createBoxField(Client *, flexbuffers::Reference data, bool) {
	flexbuffers::Vector flexVec = data.AsVector();

	std::string name            = flexVec[0].AsString().str();
	Spatial *spatialParent      = this->client->scenegraph.findNode<Spatial>(flexVec[1].AsString().str());
	sk::pose_t transform        = FlexToSKPose(flexVec[2].AsTypedVector(), flexVec[3].AsTypedVector());
	sk::vec3 size               = FlexToSKVec3(flexVec[4].AsTypedVector());

	BoxField *field = new BoxField(client, spatialParent, transform, size);
	this->addChild(name, field);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> FieldInterface::createCylinderField(Client *, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector flexVec = data.AsVector();

	std::string name            = flexVec[0].AsString().str();
	Spatial *spatialParent      = this->client->scenegraph.findNode<Spatial>(flexVec[1].AsString().str());
	sk::pose_t transform        = FlexToSKPose(flexVec[2].AsTypedVector(), flexVec[3].AsTypedVector());
	float length                = flexVec[4].AsFloat();
	float radius                = flexVec[5].AsFloat();

	CylinderField *field = new CylinderField(client, spatialParent, transform, length, radius);
	this->addChild(name, field);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> FieldInterface::createSphereField(Client *, flexbuffers::Reference data, bool) {
	flexbuffers::Vector vector = data.AsVector();

	std::string name           = vector[0].AsString().str();
	Spatial *spatialParent     = this->client->scenegraph.findNode<Spatial>(vector[1].AsString().str());
	vec3 origin                = FlexToSKVec3(vector[2].AsTypedVector());
	float radius               = vector[3].AsFloat();

	SphereField *field = new SphereField(client, spatialParent, origin, radius);
	this->addChild(name, field);

	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
