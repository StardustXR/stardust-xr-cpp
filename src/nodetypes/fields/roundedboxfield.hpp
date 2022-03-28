#pragma once

#include "boxfield.hpp"

namespace StardustXRServer {

class RoundedBoxField : public BoxField {
public:
	RoundedBoxField(Client *client, Spatial *spatialParent, sk::pose_t transform, sk::vec3 size, float radius);

	float radius;

	virtual float localDistance(const sk::vec3 point);
};

} // namespace StardustXRServer
