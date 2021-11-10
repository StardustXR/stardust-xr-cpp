#include "cylinderfield.hpp"
#include "stereokit.h"
#include "../../globals.h"

namespace StardustXRServer {

CylinderField::CylinderField(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, float length, float radius) : Field(client, spatialParent, position, rotation, true) {
	this->length = length;
	this->radius = radius;
}

float CylinderField::localDistance(const vec3 point) {
	return vec3_magnitude(point) - radius;
}

void CylinderField::debug() {
	render_add_mesh(cylinderFieldMesh, fieldDebugMat, matrix_trs(vec3_zero, quat_identity, vec3{radius, radius, length}) * worldTransform());
}

} // namespace StardustXRServer
