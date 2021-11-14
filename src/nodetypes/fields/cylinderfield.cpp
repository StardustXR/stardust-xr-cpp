#include "cylinderfield.hpp"
#include "stereokit.h"
#include "../../globals.h"

#include <cmath>

using namespace std;

namespace StardustXRServer {

CylinderField::CylinderField(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, float length, float radius) : Field(client, spatialParent, position, rotation, true) {
	this->length = length;
	this->radius = radius;
}

float CylinderField::localDistance(const vec3 point) {
	vec2 d = vec2{
		abs(vec2_magnitude(vec2{point.x,point.y})) - radius,
		abs(point.z) - (length / 2)
	};
	return min(max(d.x,d.y),0.0f) + ((d.x >= 0 && d.y >= 0) ? vec2_magnitude(d) : 0);
}

void CylinderField::debug() {
	render_add_mesh(cylinderFieldMesh, fieldDebugMat, matrix_trs(vec3_zero, quat_identity, vec3{radius, radius, length}) * worldTransform());
}

} // namespace StardustXRServer
