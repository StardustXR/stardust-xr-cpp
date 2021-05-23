#pragma once

#include "field.hpp"

namespace StardustXRServer {

class SphereField : public Field {
public:
	SphereField(float radius);
	virtual ~SphereField() {}

	float radius = 1.0f;

		 float localDistance     (const vec3 point);
	const vec3 localClosestPoint (const vec3 point);
	const vec3 localNormal       (const vec3 point);
};

} // namespace StardustXRServer
