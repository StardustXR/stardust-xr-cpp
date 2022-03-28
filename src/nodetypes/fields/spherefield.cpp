#include "spherefield.hpp"
#include "stereokit.h"
#include "../../globals.h"

using namespace sk;

namespace StardustXRServer {

SphereField::SphereField(Client *client, Spatial *spatialParent, sk::vec3 position, float radius) :
Field(client, spatialParent, pose_t{position, quat_identity}, false),
radius(radius) {}

float SphereField::localDistance(const vec3 point) {
	return vec3_magnitude(point) - radius;
}

const vec3 SphereField::localClosestPoint(const vec3 point) {
	return localNormal(point) * radius;
}

const vec3 SphereField::localNormal(const vec3 point) {
	return vec3_normalize(point);
}

void SphereField::debug() {
	render_add_mesh(sphereFieldMesh, fieldDebugMat, matrix_s(vec3_one * radius) * worldTransform());
}

} // namespace StardustXRServer
