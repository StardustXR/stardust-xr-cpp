#ifndef STARDUSTXRSERVER_SPHEREFIELD_HPP
#define STARDUSTXRSERVER_SPHEREFIELD_HPP

#include "field.hpp"

namespace StardustXRServer {

class SphereField : public Field {
public:
	SphereField();
	virtual ~SphereField() {}

	float radius = 1.0f;

		 float localDistance     (const vec3 point);
	const vec3 localClosestPoint (const vec3 point);
	const vec3 localNormal       (const vec3 point);
};

} // namespace StardustXRServer

#endif // STARDUSTXRSERVER_SPHEREFIELD_HPP
