#include "boxfield.hpp"
#include <cmath>

using namespace std;

namespace StardustXRServer {

BoxField::BoxField() {}

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

} // namespace StardustXRServer
