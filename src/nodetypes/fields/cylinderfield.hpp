#pragma once

#include "field.hpp"

namespace StardustXRServer {

class CylinderField : public Field {
public:
	CylinderField(Client *client, Spatial *spatialParent, sk::vec3 position, sk::quat rotation, float length, float radius);
	// virtual ~CylinderField();

	float length = 1.0f;
	float radius = 1.0f;

	float localDistance(const sk::vec3 point);

	void debug();
};

} // namespace StardustXRServer
