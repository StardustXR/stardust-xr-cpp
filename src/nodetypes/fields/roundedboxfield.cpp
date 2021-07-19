#include "roundedboxfield.hpp"
#include <cmath>

using namespace std;

namespace StardustXRServer {

RoundedBoxField::RoundedBoxField(Client *client, Spatial *spatialParent, vec3 position, quat rotation, vec3 size, float radius) : BoxField::BoxField(client, spatialParent, position, rotation, size) {
	this->radius = radius;
}

float RoundedBoxField::localDistance(const vec3 point) {
	float minSizeDimension = min(size.x, min(size.y, size.z));
	float clampedRadius = clamp(radius, 0.0f, minSizeDimension);

	vec3 offsetPoint = localToSpacePoint(nullptr, point);
	offsetPoint.x = abs(offsetPoint.x) - ((size.x - clampedRadius) / 2.0f);
	offsetPoint.y = abs(offsetPoint.y) - ((size.y - clampedRadius) / 2.0f);
	offsetPoint.z = abs(offsetPoint.z) - ((size.z - clampedRadius) / 2.0f);

	vec3 outsideVector = { 0.0f, 0.0f, 0.0f };
	outsideVector.x = max(offsetPoint.x, 0.0f);
	outsideVector.x = max(offsetPoint.y, 0.0f);
	outsideVector.x = max(offsetPoint.z, 0.0f);

	float outsideDistance = vec3_magnitude(outsideVector);
	float insideDistance = min(max(offsetPoint.x, max(offsetPoint.y, offsetPoint.z)), 0.0f);

	return outsideDistance + insideDistance + clampedRadius;
}

} // namespace StardustXRServer
