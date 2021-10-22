#pragma once

#include "field.hpp"

namespace StardustXRServer {

class SphereField : public Field {
public:
	SphereField(Client *client, Spatial *spatialParent, sk::vec3 position, float radius);
	// virtual ~SphereField();

	float radius = 1.0f;

		  float    localDistance     (const sk::vec3 point);
	const sk::vec3 localClosestPoint (const sk::vec3 point);
	const sk::vec3 localNormal       (const sk::vec3 point);

	void debug();
};

} // namespace StardustXRServer
