#ifndef STARDUSTXRSERVER_ROUNDEDBOXFIELD_HPP
#define STARDUSTXRSERVER_ROUNDEDBOXFIELD_HPP

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

#endif // STARDUSTXRSERVER_ROUNDEDBOXFIELD_HPP
