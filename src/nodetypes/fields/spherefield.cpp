#include "spherefield.hpp"

namespace StardustXRServer {

SphereField::SphereField(Client *client, Spatial *spatialParent, sk::vec3 position, float radius) : Field(client, spatialParent, position, quat_identity, false) {
	this->radius = radius;
	rotatable = false; // Spheres can't be rotated :p
}

float SphereField::localDistance(const vec3 point) {
	return vec3_magnitude(point) - radius;
}

const vec3 SphereField::localClosestPoint(const vec3 point) {
	return localNormal(point) * radius;
}

const vec3 SphereField::localNormal(const vec3 point) {
	return vec3_normalize(point);
}

} // namespace StardustXRServer
