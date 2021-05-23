#include "field.hpp"
#include "../../nodetypes/fields/boxfield.hpp"
#include "../../nodetypes/fields/spherefield.hpp"

using namespace sk;
using namespace std;

namespace StardustXRServer {

FieldInterface::FieldInterface() {
	STARDUSTXR_NODE_METHOD("createBoxField", &FieldInterface::createBoxField)
	STARDUSTXR_NODE_METHOD("createSphereField", &FieldInterface::createSphereField)
}

std::vector<uint8_t> FieldInterface::createBoxField(uint sessionID, flexbuffers::Reference data, bool) {
	flexbuffers::Vector vector            = data.AsVector();
	flexbuffers::TypedVector flexPosition = vector[1].AsTypedVector();
	flexbuffers::TypedVector flexRotation = vector[2].AsTypedVector();
	flexbuffers::TypedVector flexSize     = vector[3].AsTypedVector();

	string name = vector[0].AsString().str();
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

	BoxField *field = new BoxField(size);
	field->ready = false;
	field->sessionID = sessionID;
	field->position = position;
	field->rotation = rotation;
	field->transformDirty();
	children[name] = field;
	field->ready = true;

	return std::vector<uint8_t>();
}

std::vector<uint8_t> FieldInterface::createSphereField(uint sessionID, flexbuffers::Reference data, bool) {
	flexbuffers::Vector vector          = data.AsVector();
	flexbuffers::TypedVector flexOrigin = vector[1].AsTypedVector();

	string name = vector[0].AsString().str();
	vec3 origin = {
		flexOrigin[0].AsFloat(),
		flexOrigin[1].AsFloat(),
		flexOrigin[2].AsFloat()
	};
	float radius = vector[2].AsFloat();

	SphereField *field = new SphereField(radius);
	field->ready = false;
	field->sessionID = sessionID;
	field->position = origin;
	field->transformDirty();
	children[name] = field;
	field->ready = true;

	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
