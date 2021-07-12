#pragma once

#include "boxfield.hpp"

namespace StardustXRServer {

class RoundedBoxField : public BoxField {
public:
	RoundedBoxField(Client *client, vec3 size, float radius);
	virtual ~RoundedBoxField() {}

	float radius;

	virtual float localDistance(const vec3 point);
};

} // namespace StardustXRServer
