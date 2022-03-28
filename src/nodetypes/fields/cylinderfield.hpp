#pragma once

#include "field.hpp"

namespace StardustXRServer {

class CylinderField : public Field {
public:
	CylinderField(Client *client, Spatial *spatialParent, pose_t pose, float length, float radius);

	float length = 1.0f;
	float radius = 1.0f;

	float localDistance(const sk::vec3 point);

	void debug();
};

} // namespace StardustXRServer
