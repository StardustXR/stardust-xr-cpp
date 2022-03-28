#pragma once

#include "boxfield.hpp"

namespace StardustXRServer {

class RoundedBoxField : public BoxField {
public:
	RoundedBoxField(Client *client, Spatial *spatialParent, pose_t transform, vec3 size, float radius);

	float radius;

	virtual float localDistance(const vec3 point);
};

} // namespace StardustXRServer
