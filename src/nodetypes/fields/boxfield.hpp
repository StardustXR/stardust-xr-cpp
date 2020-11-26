#ifndef STARDUSTXRSERVER_BOXFIELD_HPP
#define STARDUSTXRSERVER_BOXFIELD_HPP

#include "field.hpp"

namespace StardustXRServer {

class BoxField : public Field {
public:
	BoxField();
	virtual ~BoxField() {}

	vec3 size;

	float localDistance(const vec3 point);
};

} // namespace StardustXRServer

#endif // STARDUSTXRSERVER_BOXFIELD_HPP
