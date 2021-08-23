#include "boxfield.hpp"
#include <cmath>
#include "stereokit.h"
#include "../../globals.h"

using namespace std;

namespace StardustXRServer {

BoxField::BoxField(Client *client, Spatial *spatialParent, vec3 position, quat rotation, vec3 size) : Field(client, spatialParent, position, rotation, true) {
	this->size = size;
	
	STARDUSTXR_NODE_METHOD("setSize", &BoxField::setSize)
}

std::vector<uint8_t> BoxField::setSize(flexbuffers::Reference data, bool) {
	flexbuffers::TypedVector vector = data.AsTypedVector();
	size = { vector[0].AsFloat(), vector[1].AsFloat(), vector[2].AsFloat() };

	return std::vector<uint8_t>();
}

float BoxField::localDistance(const vec3 point) {
	vec3 q = {
		std::abs(point.x) - (size.x * 0.5f),
		std::abs(point.y) - (size.y * 0.5f),
		std::abs(point.z) - (size.z * 0.5f)
	};
	vec3 v = {
		std::max(q.x, 0.0f),
		std::max(q.y, 0.0f),
		std::max(q.z, 0.0f)
	};

	return vec3_magnitude(v) + std::min(std::max(q.x, std::max(q.y, q.z)), 0.0f);
}

void BoxField::debug() {
	render_add_mesh(boxFieldMesh, fieldDebugMat, matrix_trs(vec3_zero, quat_identity, size) * worldTransform());
}

} // namespace StardustXRServer
