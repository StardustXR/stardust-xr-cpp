#include "roundedboxfield.hpp"
#include <cmath>

using namespace sk;

namespace StardustXRServer {

RoundedBoxField::RoundedBoxField(Client *client, Spatial *spatialParent, pose_t transform, vec3 size, float radius) :
BoxField::BoxField(client, spatialParent, transform, size),
radius(radius) {}

float RoundedBoxField::localDistance(const vec3 point) {
	float minSizeDimension = std::min(size.x, std::min(size.y, size.z));
	float clampedRadius = std::clamp(radius, 0.0f, minSizeDimension);

	vec3 offsetPoint = localToSpacePoint(nullptr, point);
	offsetPoint.x = abs(offsetPoint.x) - ((size.x - clampedRadius) / 2.0f);
	offsetPoint.y = abs(offsetPoint.y) - ((size.y - clampedRadius) / 2.0f);
	offsetPoint.z = abs(offsetPoint.z) - ((size.z - clampedRadius) / 2.0f);

	vec3 outsideVector = { 0.0f, 0.0f, 0.0f };
	outsideVector.x = std::max(offsetPoint.x, 0.0f);
	outsideVector.x = std::max(offsetPoint.y, 0.0f);
	outsideVector.x = std::max(offsetPoint.z, 0.0f);

	float outsideDistance = vec3_magnitude(outsideVector);
	float insideDistance = std::min(std::max(offsetPoint.x, std::max(offsetPoint.y, offsetPoint.z)), 0.0f);

	return outsideDistance + insideDistance + clampedRadius;
}

} // namespace StardustXRServer
