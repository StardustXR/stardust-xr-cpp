#include "spatial.hpp"
#include "../nodetypes/fields/boxfield.hpp"
#include "../nodetypes/fields/spherefield.hpp"

using namespace sk;
using namespace std;

namespace StardustXRServer {

SpatialFactory::SpatialFactory(Client *client) : Node(client) {
	STARDUSTXR_NODE_METHOD("create", &SpatialFactory::create)
}

std::vector<uint8_t> SpatialFactory::create(flexbuffers::Reference data, bool) {
	flexbuffers::Vector vector            = data.AsVector();
	flexbuffers::TypedVector flexPosition = vector[1].AsTypedVector();
	flexbuffers::TypedVector flexRotation = vector[2].AsTypedVector();
	flexbuffers::TypedVector flexScale    = vector[3].AsTypedVector();
	bool translatable                     = vector[4].AsBool();
	bool rotatable                        = vector[5].AsBool();
	bool scalable                         = vector[6].AsBool();

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
	vec3 scale = {
		flexScale[0].AsFloat(),
		flexScale[1].AsFloat(),
		flexScale[2].AsFloat()
	};

	Spatial *spatial = new Spatial(client);
	spatial->ready = false;
	spatial->position = position;
	spatial->rotation = rotation;
	spatial->scale = scale;
	spatial->translatable = translatable;
	spatial->rotatable = rotatable;
	spatial->scalable = scalable;
	spatial->transformDirty();
	children.emplace(name, spatial);
	spatial->ready = true;

	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
