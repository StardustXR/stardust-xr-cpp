#pragma once

#include "boxfield.hpp"

namespace StardustXRServer {

class RoundedBoxField : public BoxField {
public:
	RoundedBoxField();
	virtual ~RoundedBoxField() {}

	float radius;

	virtual float localDistance(const vec3 point);
};

} // namespace StardustXRServer
