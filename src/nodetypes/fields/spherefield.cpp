#include "spherefield.hpp"

namespace StardustXRServer {

SphereField::SphereField() {
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
