#pragma once

#include "field.hpp"

namespace StardustXRServer {

class BoxField : public Field {
public:
	BoxField();
	virtual ~BoxField() {}

	vec3 size;

	virtual float localDistance(const vec3 point);
};

} // namespace StardustXRServer
