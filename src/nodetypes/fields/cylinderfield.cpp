#include "cylinderfield.hpp"
#include "stereokit.h"
#include "../../globals.hpp"

#include <cmath>

using namespace sk;

namespace StardustXRServer {

CylinderField::CylinderField(Client *client, Spatial *spatialParent, pose_t pose, float length, float radius) :
Field(client, spatialParent, pose, true) {
	this->length = length;
	this->radius = radius;
}

float CylinderField::localDistance(const vec3 point) {
	vec2 d = vec2{
		abs(vec2_magnitude(vec2{point.x,point.y})) - radius,
		abs(point.z) - (length / 2)
	};
	return std::min(std::max(d.x,d.y),0.0f) + ((d.x >= 0 && d.y >= 0) ? vec2_magnitude(d) : 0);
}

void CylinderField::debug() {
	render_add_mesh(cylinderFieldMesh, fieldDebugMat, matrix_s(vec3{radius, radius, length}) * worldTransform());
}

} // namespace StardustXRServer
