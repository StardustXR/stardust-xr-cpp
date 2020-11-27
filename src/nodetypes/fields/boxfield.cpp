#include "boxfield.hpp"
#include <cmath>

using namespace std;

namespace StardustXRServer {

BoxField::BoxField() {}

float BoxField::localDistance(const vec3 point) {
	vec3 offsetPoint = localToSpacePoint(nullptr, point);
	offsetPoint.x = abs(offsetPoint.x) - (size.x / 2.0f);
	offsetPoint.y = abs(offsetPoint.y) - (size.y / 2.0f);
	offsetPoint.z = abs(offsetPoint.z) - (size.z / 2.0f);

	vec3 outsideVector = { 0.0f, 0.0f, 0.0f };
	outsideVector.x = max(offsetPoint.x, 0.0f);
	outsideVector.x = max(offsetPoint.y, 0.0f);
	outsideVector.x = max(offsetPoint.z, 0.0f);

	float outsideDistance = vec3_magnitude(outsideVector);
	float insideDistance = min(max(offsetPoint.x, max(offsetPoint.y, offsetPoint.z)), 0.0f);

	return outsideDistance + insideDistance;
}

} // namespace StardustXRServer
