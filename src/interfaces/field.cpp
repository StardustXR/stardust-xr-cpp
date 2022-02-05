#include "field.hpp"
#include "../nodetypes/fields/boxfield.hpp"
#include "../nodetypes/fields/cylinderfield.hpp"
#include "../nodetypes/fields/spherefield.hpp"
#include "../core/client.hpp"

using namespace sk;
using namespace std;

namespace StardustXRServer {

FieldInterface::FieldInterface(Client *client) : Node(client, false) {
	STARDUSTXR_NODE_METHOD("createBoxField", &FieldInterface::createBoxField)
	STARDUSTXR_NODE_METHOD("createCylinderField", &FieldInterface::createCylinderField)
	STARDUSTXR_NODE_METHOD("createSphereField", &FieldInterface::createSphereField)
}

std::vector<uint8_t> FieldInterface::createBoxField(Client *, flexbuffers::Reference data, bool) {
	flexbuffers::Vector vector            = data.AsVector();

	string name                           = vector[0].AsString().str();
	Spatial *spatialParent                = this->client->scenegraph.findNode<Spatial>(vector[1].AsString().str());
	flexbuffers::TypedVector flexPosition = vector[2].AsTypedVector();
	flexbuffers::TypedVector flexRotation = vector[3].AsTypedVector();
	flexbuffers::TypedVector flexSize     = vector[4].AsTypedVector();
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
	vec3 size = {
		flexSize[0].AsFloat(),
		flexSize[1].AsFloat(),
		flexSize[2].AsFloat()
	};

	BoxField *field = new BoxField(client, spatialParent, position, rotation, size);
	this->addChild(name, field);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> FieldInterface::createCylinderField(Client *, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector vector          = data.AsVector();

	string name                         = vector[0].AsString().str();
	Spatial *spatialParent              = this->client->scenegraph.findNode<Spatial>(vector[1].AsString().str());
	flexbuffers::TypedVector flexOrigin = vector[2].AsTypedVector();
	flexbuffers::TypedVector flexRotation = vector[3].AsTypedVector();
	float length                        = vector[4].AsFloat();
	float radius                        = vector[5].AsFloat();

	vec3 origin = {
		flexOrigin[0].AsFloat(),
		flexOrigin[1].AsFloat(),
		flexOrigin[2].AsFloat()
	};
	quat rotation = {
		flexRotation[0].AsFloat(),
		flexRotation[1].AsFloat(),
		flexRotation[2].AsFloat(),
		flexRotation[3].AsFloat()
	};

	CylinderField *field = new CylinderField(client, spatialParent, origin, rotation, length, radius);
	this->addChild(name, field);

	return std::vector<uint8_t>();
}

std::vector<uint8_t> FieldInterface::createSphereField(Client *, flexbuffers::Reference data, bool) {
	flexbuffers::Vector vector          = data.AsVector();

	string name                         = vector[0].AsString().str();
	Spatial *spatialParent              = this->client->scenegraph.findNode<Spatial>(vector[1].AsString().str());
	flexbuffers::TypedVector flexOrigin = vector[2].AsTypedVector();
	float radius                        = vector[3].AsFloat();

	vec3 origin = {
		flexOrigin[0].AsFloat(),
		flexOrigin[1].AsFloat(),
		flexOrigin[2].AsFloat()
	};

	SphereField *field = new SphereField(client, spatialParent, origin, radius);
	this->addChild(name, field);

	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
