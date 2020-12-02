#include "field.hpp"
#include "../../nodetypes/fields/boxfield.hpp"
#include "../../nodetypes/fields/spherefield.hpp"

using namespace sk;
using namespace std;

namespace StardustXRServer {

FieldInterface::FieldInterface() {
	STARDUSTXR_NODE_METHOD("createSphereField", &FieldInterface::createSphereField)
}

std::vector<uint8_t> FieldInterface::createSphereField(uint sessionID, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector vector          = data.AsVector();
	flexbuffers::TypedVector flexOrigin = vector[1].AsTypedVector();

	string name = vector[0].AsString().str();
	vec3 origin = {
		flexOrigin[0].AsFloat(),
		flexOrigin[1].AsFloat(),
		flexOrigin[2].AsFloat()
	};
	float radius = vector[2].AsFloat();

	SphereField *field = new SphereField();
	field->ready = false;
	field->sessionID = sessionID;
	field->position = origin;
	field->transformDirty();
	field->radius = radius;
	children[name] = field;
	field->ready = true;

	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
